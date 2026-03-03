# pi-fleet

Shared codebase for Buildmo Raspberry Pi fleet (pi-1, pi-2, pi-3).

## Quick Start

1. Clone: `git clone git@github.com:buildmo/pi-fleet.git`
2. Pi setup: `ssh pi@<hostname>` then `git clone` + `bash scripts/setup.sh`
3. Dev on laptop, push to GitHub, Pis auto-pull.

## Config

Each Pi loads `configs/<hostname>.yaml` automatically at runtime.
