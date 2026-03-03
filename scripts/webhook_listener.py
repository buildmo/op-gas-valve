#!/usr/bin/env python3
"""
GitHub Webhook Listener
Listens for push events and triggers deployment.
Runs as a systemd service on each Pi.
"""

import hashlib
import hmac
import json
import logging
import os
import subprocess
from http.server import HTTPServer, BaseHTTPRequestHandler

WEBHOOK_PORT = int(os.environ.get("WEBHOOK_PORT", 9000))
WEBHOOK_SECRET = os.environ.get("WEBHOOK_SECRET", "")
REPO_DIR = os.environ.get("REPO_DIR", os.path.expanduser("~/op-gas-valve"))
DEPLOY_SCRIPT = os.path.join(REPO_DIR, "scripts", "deploy.sh")
BRANCH = os.environ.get("DEPLOY_BRANCH", "main")

logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s [%(levelname)s] %(message)s"
)
log = logging.getLogger("webhook")


def verify_signature(payload, signature):
    if not WEBHOOK_SECRET:
        log.warning("No WEBHOOK_SECRET set - skipping signature verification")
        return True
    if not signature:
        return False
    expected = "sha256=" + hmac.new(
        WEBHOOK_SECRET.encode(), payload, hashlib.sha256
    ).hexdigest()
    return hmac.compare_digest(expected, signature)


class WebhookHandler(BaseHTTPRequestHandler):
    def do_POST(self):
        content_length = int(self.headers.get("Content-Length", 0))
        payload = self.rfile.read(content_length)

        signature = self.headers.get("X-Hub-Signature-256", "")
        if not verify_signature(payload, signature):
            log.warning("Invalid signature - rejecting request")
            self.send_response(403)
            self.end_headers()
            self.wfile.write(b"Invalid signature")
            return

        try:
            data = json.loads(payload)
        except json.JSONDecodeError:
            self.send_response(400)
            self.end_headers()
            self.wfile.write(b"Invalid JSON")
            return

        ref = data.get("ref", "")
        if ref == f"refs/heads/{BRANCH}":
            log.info(f"Push to {BRANCH} detected - deploying...")
            try:
                result = subprocess.run(
                    ["bash", DEPLOY_SCRIPT],
                    capture_output=True, text=True, timeout=120
                )
                log.info(f"Deploy stdout: {result.stdout}")
                if result.returncode != 0:
                    log.error(f"Deploy stderr: {result.stderr}")
                self.send_response(200)
                self.end_headers()
                self.wfile.write(b"Deployed")
            except subprocess.TimeoutExpired:
                log.error("Deploy timed out")
                self.send_response(500)
                self.end_headers()
                self.wfile.write(b"Deploy timeout")
        else:
            log.info(f"Ignoring push to {ref} (not {BRANCH})")
            self.send_response(200)
            self.end_headers()
            self.wfile.write(b"Ignored")

    def do_GET(self):
        self.send_response(200)
        self.end_headers()
        self.wfile.write(b"Webhook listener is running")

    def log_message(self, format, *args):
        pass  # suppress default HTTP logs


if __name__ == "__main__":
    server = HTTPServer(("0.0.0.0", WEBHOOK_PORT), WebhookHandler)
    log.info(f"Webhook listener started on port {WEBHOOK_PORT}")
    log.info(f"Watching branch: {BRANCH}")
    log.info(f"Repo dir: {REPO_DIR}")
    server.serve_forever()
