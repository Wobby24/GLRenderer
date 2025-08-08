#!/bin/sh
set -e

OS="$(uname)"

if [ "$OS" = "Linux" ]; then
    if [ -f /etc/os-release ]; then
        . /etc/os-release
        case "$ID" in
            arch)
                ./setup-linux/setup-arch.sh
                ;;
            debian|ubuntu)
                ./setup-linux/setup-debian.sh
                ;;
            *)
                echo "Unsupported Linux distro: $ID"
                exit 1
                ;;
        esac
    fi
elif [ "$OS" = "Darwin" ]; then
    echo "macOS setup not yet implemented."
else
    echo "Unsupported OS: $OS"
    exit 1
fi
