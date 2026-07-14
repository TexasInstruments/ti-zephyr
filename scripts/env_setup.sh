#!/bin/bash
# TI Zephyr SDK environment setup — Linux/macOS
# Usage: source env_setup.sh
eval "$(python3 "$(dirname "${BASH_SOURCE[0]}")/env_setup.py")"
echo "TI Zephyr SDK environment ready."
echo "  ZEPHYR_BASE=$ZEPHYR_BASE"
echo "  ZEPHYR_SDK_INSTALL_DIR=$ZEPHYR_SDK_INSTALL_DIR"
