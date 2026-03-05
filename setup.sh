#!/bin/bash
# ================================================================
# OGV Pi Setup Script
# ================================================================
# Installs all software needed for the robotics challenge:
#   - Python packages (guizero, pyserial)
#   - tkinter (GUI framework)
#   - arduino-cli (compile + upload Arduino sketches)
#
# Usage:
#   bash setup.sh
# ================================================================

set -e

GREEN='\033[0;32m'
RED='\033[0;31m'
BOLD='\033[1m'
NC='\033[0m'
CHECK="${GREEN}✓${NC}"
CROSS="${RED}✗${NC}"

echo ""
echo -e "${BOLD}═══ OGV Pi Setup ═══${NC}"
echo ""

# ── Python packages ──────────────────────────────────────

echo -e "${BOLD}Installing Python dependencies...${NC}"

echo "  Updating apt..."
sudo apt-get update -qq

printf "  python3-tk (tkinter)..."
if sudo apt-get install -y python3-tk -qq > /dev/null 2>&1; then
    echo -e " ${CHECK}"
else
    echo -e " ${CROSS}"
fi

printf "  guizero..."
if pip3 install guizero -q > /dev/null 2>&1; then
    echo -e " ${CHECK}"
else
    echo -e " ${CROSS}"
fi

printf "  pyserial..."
if pip3 install pyserial -q > /dev/null 2>&1; then
    echo -e " ${CHECK}"
else
    echo -e " ${CROSS}"
fi

# ── Arduino CLI ──────────────────────────────────────────

echo ""
echo -e "${BOLD}Installing arduino-cli...${NC}"

if command -v arduino-cli > /dev/null 2>&1; then
    echo -e "  Already installed ${CHECK}"
else
    printf "  Downloading..."
    if curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh > /dev/null 2>&1; then
        sudo mv bin/arduino-cli /usr/local/bin/
        rmdir bin 2>/dev/null || true
        echo -e " ${CHECK}"
    else
        echo -e " ${CROSS}"
    fi
fi

printf "  Arduino AVR core..."
if arduino-cli core install arduino:avr > /dev/null 2>&1; then
    echo -e " ${CHECK}"
else
    echo -e " ${CROSS}"
fi

# ── Verify ───────────────────────────────────────────────

echo ""
echo -e "${BOLD}Verifying...${NC}"

printf "  python3"
if command -v python3 > /dev/null 2>&1; then
    echo -e " $(python3 --version 2>&1 | cut -d' ' -f2) ${CHECK}"
else
    echo -e " ${CROSS}"
fi

printf "  guizero"
if python3 -c "import guizero" > /dev/null 2>&1; then
    echo -e " ${CHECK}"
else
    echo -e " ${CROSS}"
fi

printf "  pyserial"
if python3 -c "import serial" > /dev/null 2>&1; then
    echo -e " ${CHECK}"
else
    echo -e " ${CROSS}"
fi

printf "  tkinter"
if python3 -c "import tkinter" > /dev/null 2>&1; then
    echo -e " ${CHECK}"
else
    echo -e " ${CROSS}"
fi

printf "  arduino-cli"
if command -v arduino-cli > /dev/null 2>&1; then
    echo -e " $(arduino-cli version 2>&1 | grep -oP '[\d.]+' | head -1) ${CHECK}"
else
    echo -e " ${CROSS}"
fi

echo ""
echo -e "${BOLD}═══ Setup complete ═══${NC}"
echo ""
echo "Next: run 'bash test_milestones.sh' to verify all code compiles."
echo ""
