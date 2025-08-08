#!/bin/bash
set -e

echo "Installing dependencies for Debian..."
sudo apt update
sudo apt install -y build-essential cmake git \
    libgl1-mesa-dev libx11-dev libxrandr-dev libxi-dev libxinerama-dev \
    libxcursor-dev libwayland-dev xorg-dev pkg-config
