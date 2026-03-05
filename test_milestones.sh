#!/bin/bash
# ================================================================
# OGV Milestone Test Runner
# ================================================================
# Compiles every Arduino sketch and syntax-checks every Python
# file across all milestones + final code.
#
# Usage:
#   bash test_milestones.sh              # compile + syntax only
#   bash test_milestones.sh --upload     # also upload each sketch
#
# Environment:
#   BOARD  — Arduino FQBN (default: arduino:avr:uno)
#   PORT   — Serial port  (default: /dev/ttyACM0)
# ================================================================

set -euo pipefail

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BOLD='\033[1m'
DIM='\033[2m'
NC='\033[0m'
CHECK="${GREEN}✓${NC}"
CROSS="${RED}✗${NC}"

BOARD="${BOARD:-arduino:avr:uno}"
PORT="${PORT:-/dev/ttyACM0}"
UPLOAD=false
DIR="$(cd "$(dirname "$0")" && pwd)"

pass=0
fail=0

[[ "${1:-}" == "--upload" ]] && UPLOAD=true

# ── Helpers ──────────────────────────────────────────────

compile_sketch() {
    local sketch="$1"
    local name
    name="$(basename "$sketch")"
    printf "  Compile  %-32s" "$name"
    if arduino-cli compile --fqbn "$BOARD" "$sketch" > /dev/null 2>&1; then
        echo -e " ${CHECK}"
        ((pass++))
    else
        echo -e " ${CROSS}"
        ((fail++))
    fi
}

upload_sketch() {
    local sketch="$1"
    local name
    name="$(basename "$sketch")"
    if $UPLOAD; then
        printf "  Upload   %-32s" "$name"
        if arduino-cli upload -p "$PORT" --fqbn "$BOARD" "$sketch" > /dev/null 2>&1; then
            echo -e " ${CHECK}"
            ((pass++))
        else
            echo -e " ${CROSS}"
            ((fail++))
        fi
    fi
}

check_python() {
    local file="$1"
    local name
    name="$(basename "$file")"
    printf "  Syntax   %-32s" "$name"
    if python3 -c "import py_compile; py_compile.compile('$file', doraise=True)" > /dev/null 2>&1; then
        echo -e " ${CHECK}"
        ((pass++))
    else
        echo -e " ${CROSS}"
        ((fail++))
    fi
}

test_ino() {
    local sketch_dir="$1"
    compile_sketch "$sketch_dir"
    upload_sketch "$sketch_dir"
}

section() {
    echo ""
    echo -e "${BOLD}── $1 ──${NC}"
    echo ""
}

label() {
    echo -e "  ${DIM}[$1]${NC} $2"
}

# ── Prerequisites ────────────────────────────────────────

echo ""
echo -e "${BOLD}═══ OGV MILESTONE TEST RUNNER ═══${NC}"

section "Prerequisites"

printf "  arduino-cli %29s" ""
if command -v arduino-cli > /dev/null 2>&1; then
    echo -e " ${CHECK}"
else
    echo -e " ${CROSS}  (install: https://arduino.github.io/arduino-cli)"
fi

printf "  python3 %33s" ""
if command -v python3 > /dev/null 2>&1; then
    echo -e " ${CHECK}"
else
    echo -e " ${CROSS}"
fi

printf "  guizero %33s" ""
if python3 -c "import guizero" > /dev/null 2>&1; then
    echo -e " ${CHECK}"
else
    echo -e " ${CROSS}  (pip3 install guizero)"
fi

printf "  pyserial %32s" ""
if python3 -c "import serial" > /dev/null 2>&1; then
    echo -e " ${CHECK}"
else
    echo -e " ${CROSS}  (pip3 install pyserial)"
fi

if $UPLOAD; then
    printf "  Serial port %-28s" "$PORT"
    if [ -e "$PORT" ]; then
        echo -e " ${CHECK}"
    else
        echo -e " ${CROSS}  (not found)"
    fi
fi

# ── Day 1 HR5 ───────────────────────────────────────────

section "Day 1 HR5"

label "TANK" "1_motor_test"
test_ino "$DIR/milestones/tank/1_motor_test"

label "ARM " "1_servo_test"
test_ino "$DIR/milestones/arm/1_servo_test"

label "PILOT" "1_tank_gui"
check_python "$DIR/milestones/pilot/1_tank_gui.py"

# ── Day 1 HR7 ───────────────────────────────────────────

section "Day 1 HR7"

label "PILOT" "1b_gui_framework"
check_python "$DIR/milestones/pilot/1b_gui_framework.py"

# ── Day 2 HR3 (Hard Deadline) ───────────────────────────

section "Day 2 HR3 — Hard Deadline"

label "TANK" "2_serial_control"
test_ino "$DIR/milestones/tank/2_serial_control"

label "ARM " "2_serial_control"
test_ino "$DIR/milestones/arm/2_serial_control"

label "PILOT" "2_tank_burst_gui"
check_python "$DIR/milestones/pilot/2_tank_burst_gui.py"

# ── Day 2 HR5 (Integration) ─────────────────────────────

section "Day 2 HR5 — Integration"

label "COMBINED" "robot_controller"
test_ino "$DIR/milestones/combined/robot_controller"

label "PILOT" "3_full_gui"
check_python "$DIR/milestones/pilot/3_full_gui.py"

# ── Final ────────────────────────────────────────────────

section "Final (production code)"

label "ARDUINO" "robot_controller"
test_ino "$DIR/final_arduino/robot_controller"

label "GUI" "gui.py"
check_python "$DIR/final_gui/gui.py"

label "GUI" "main.py"
check_python "$DIR/final_gui/main.py"

# ── Summary ──────────────────────────────────────────────

echo ""
echo -e "${BOLD}════════════════════════════════════${NC}"
if [ "$fail" -eq 0 ]; then
    echo -e "  ${GREEN}${pass} passed${NC}  ${DIM}0 failed${NC}"
else
    echo -e "  ${GREEN}${pass} passed${NC}  ${RED}${fail} failed${NC}"
fi
echo -e "${BOLD}════════════════════════════════════${NC}"
echo ""

exit "$fail"
