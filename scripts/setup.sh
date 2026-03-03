#!/bin/bash
# One-time setup for each Raspberry Pi
set -e

REPO_URL="https://github.com/buildmo/op-gas-valve.git"
REPO_DIR="$HOME/op-gas-valve"
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

# Allow pi user to restart services without password
echo "Setting up passwordless restart..."
sudo tee /etc/sudoers.d/op-gas-valve > /dev/null << SUDOEOF
$PI_USER ALL=(ALL) NOPASSWD: /bin/systemctl restart op-gas-valve
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

# Create systemd service for the poller
sudo tee /etc/systemd/system/op-gas-valve-poller.service > /dev/null << POLLEOF
[Unit]
Description=GitHub Poller for Op Gas Valve
After=network-online.target
Wants=network-online.target

[Service]
Type=simple
User=$PI_USER
Environment=REPO_DIR=$REPO_DIR
Environment=DEPLOY_BRANCH=main
Environment=POLL_INTERVAL=60
ExecStart=/bin/bash $REPO_DIR/scripts/poll_and_deploy.sh
Restart=always
RestartSec=10

[Install]
WantedBy=multi-user.target
POLLEOF

# Enable and start services
sudo systemctl daemon-reload
sudo systemctl enable op-gas-valve op-gas-valve-poller
sudo systemctl start op-gas-valve-poller

echo ""
echo "=== Setup Complete ==="
echo "Poller running - checks GitHub every 60 seconds"
echo "Main app:  sudo systemctl status op-gas-valve"
echo "Poller:    sudo systemctl status op-gas-valve-poller"
echo "Logs:      journalctl -u op-gas-valve-poller -f"
