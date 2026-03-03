#!/usr/bin/env bash
set -e

PORT="${1:-/dev/ttyACM0}"
FQBN="arduino:avr:uno"
SKETCH="arduino/robot_controller"

echo "Compiling sketch..."
arduino-cli compile --fqbn "$FQBN" "$SKETCH"

echo "Uploading to $PORT..."
arduino-cli upload -p "$PORT" --fqbn "$FQBN" "$SKETCH"

echo "Done. Arduino is ready."
