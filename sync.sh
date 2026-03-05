#!/bin/bash
# ================================================================
# AUTO-SYNC — Pull from GitHub every 60 seconds
# ================================================================
# Usage:
#   bash sync.sh              # run in foreground (see live log)
#   bash sync.sh &            # run in background
#   bash sync.sh 2>&1 | tee sync.log   # run with log file
#
# Stop:
#   Ctrl+C (foreground) or: kill $(pgrep -f sync.sh)
# ================================================================

set -uo pipefail

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR"

INTERVAL=60

GREEN='\033[0;32m'
YELLOW='\033[1;33m'
DIM='\033[2m'
NC='\033[0m'

echo -e "${GREEN}Auto-sync started${NC} — pulling every ${INTERVAL}s"
echo -e "${DIM}Repo: $DIR${NC}"
echo ""

while true; do
    timestamp=$(date '+%H:%M:%S')
    output=$(git pull 2>&1)

    if echo "$output" | grep -q "Already up to date"; then
        echo -e "${DIM}[$timestamp] No changes${NC}"
    else
        echo -e "${GREEN}[$timestamp] Updated:${NC}"
        echo "$output"
    fi

    sleep "$INTERVAL"
done
