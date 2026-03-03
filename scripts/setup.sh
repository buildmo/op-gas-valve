#!/bin/bash
# One-time setup for each Raspberry Pi
# Run as: bash scripts/setup.sh
set -e

REPO_URL="https://github.com/buildmo/op-gas-valve.git"
REPO_DIR="$HOME/op-gas-valve"
WEBHOOK_PORT=9000
PI_USER=$(whoami)

echo "=== Op Gas Valve - Pi Setup ==="

# Install dependencies
echo "Installing system packages..."
sudo apt-get update -qq
sudo apt-get install -y python3 python3-pip git

# Clone repo if not already present
if [ ! -d "$REPO_DIR" ]; then
    echo "Cloning repo..."
    git clone "$REPO_URL" "$REPO_DIR"
else
    echo "Repo already exists, pulling latest..."
    cd "$REPO_DIR" && git pull origin main
fi

cd "$REPO_DIR"
pip3 install -r requirements.txt --break-system-packages -q

# Prompt for webhook secret
read -p "Enter your GitHub webhook secret (or press Enter to skip): " WEBHOOK_SECRET

# Create .env file for the webhook listener
cat > "$REPO_DIR/.env" << ENVEOF
WEBHOOK_PORT=$WEBHOOK_PORT
WEBHOOK_SECRET=$WEBHOOK_SECRET
REPO_DIR=$REPO_DIR
DEPLOY_BRANCH=main
ENVEOF

# Allow pi user to restart services without password
echo "Setting up passwordless restart..."
sudo tee /etc/sudoers.d/op-gas-valve > /dev/null << SUDOEOF
$PI_USER ALL=(ALL) NOPASSWD: /bin/systemctl restart op-gas-valve
$PI_USER ALL=(ALL) NOPASSWD: /bin/systemctl restart op-gas-valve-webhook
SUDOEOF

# Create systemd service for the main app
sudo tee /etc/systemd/system/op-gas-valve.service > /dev/null << SVCEOF
[Unit]
Description=Op Gas Valve Application
After=network.target

[Service]
Type=simple
User=$PI_USER
WorkingDirectory=$REPO_DIR
ExecStart=/usr/bin/python3 src/main.py
Restart=on-failure
RestartSec=5

[Install]
WantedBy=multi-user.target
SVCEOF

# Create systemd service for the webhook listener
sudo tee /etc/systemd/system/op-gas-valve-webhook.service > /dev/null << WHEOF
[Unit]
Description=GitHub Webhook Listener
After=network.target

[Service]
Type=simple
User=$PI_USER
WorkingDirectory=$REPO_DIR
EnvironmentFile=$REPO_DIR/.env
ExecStart=/usr/bin/python3 scripts/webhook_listener.py
Restart=always
RestartSec=5

[Install]
WantedBy=multi-user.target
WHEOF

# Enable and start services
sudo systemctl daemon-reload
sudo systemctl enable op-gas-valve op-gas-valve-webhook
sudo systemctl start op-gas-valve-webhook

echo ""
echo "=== Setup Complete ==="
echo "Webhook listener running on port $WEBHOOK_PORT"
echo "Main app service: sudo systemctl status op-gas-valve"
echo "Webhook service:  sudo systemctl status op-gas-valve-webhook"
echo ""
echo "Next: Add a webhook in GitHub repo settings:"
echo "  URL: http://<this-pi-ip>:$WEBHOOK_PORT"
echo "  Content type: application/json"
echo "  Secret: (the one you entered above)"
echo "  Events: Just the push event"
