#!/bin/bash
# Polls GitHub for new commits and deploys if changed
# Runs as a systemd service on each Pi

REPO_DIR="${REPO_DIR:-$HOME/op-gas-valve}"
BRANCH="${DEPLOY_BRANCH:-main}"
POLL_INTERVAL="${POLL_INTERVAL:-60}"
APP_SERVICE="op-gas-valve"

cd "$REPO_DIR" || exit 1

echo "Polling started: checking $BRANCH every ${POLL_INTERVAL}s"

while true; do
    # Get latest remote commit
    git fetch origin "$BRANCH" -q 2>/dev/null

    LOCAL=$(git rev-parse HEAD)
    REMOTE=$(git rev-parse "origin/$BRANCH")

    if [ "$LOCAL" != "$REMOTE" ]; then
        echo "$(date) - New commit detected, deploying..."
        git reset --hard "origin/$BRANCH"

        echo "Installing dependencies..."
        pip3 install -r requirements.txt --break-system-packages -q

        echo "Restarting $APP_SERVICE..."
        sudo systemctl restart "$APP_SERVICE"

        echo "$(date) - Deploy complete: $(git log --oneline -1)"
    fi

    sleep "$POLL_INTERVAL"
done
