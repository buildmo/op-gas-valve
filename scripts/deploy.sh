#!/bin/bash
# Deployment script - called by webhook listener
# Pulls latest code, installs deps, restarts the app
set -e

REPO_DIR="${REPO_DIR:-$HOME/op-gas-valve}"
APP_SERVICE="op-gas-valve"

cd "$REPO_DIR"

echo "Pulling latest code..."
git fetch origin main
git reset --hard origin/main

echo "Installing Python dependencies..."
pip3 install -r requirements.txt --break-system-packages -q

echo "Restarting $APP_SERVICE service..."
sudo systemctl restart "$APP_SERVICE"

echo "Deploy complete at $(date)"
