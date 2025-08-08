#!/bin/sh
set -e

OS="$(uname)"

case "$OS" in
    Linux)
        echo "[INFO] Detected Linux"
        ./setup-scripts/setup-linux.sh
        ;;
    Darwin)
        echo "[INFO] Detected macOS"
        ./setup-scripts/setup-macos.sh
        ;;
    *)
        echo "[ERROR] Unsupported Operating System: $OS"
        exit 1
        ;;
esac
