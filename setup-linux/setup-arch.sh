#!/bin/bash
set -e

echo "Installing dependencies for Arch..."
sudo pacman -Syu --noconfirm
sudo pacman -S --noconfirm base-devel cmake git \
    mesa libx11 libxrandr libxi libxinerama libxcursor wayland pkgconf
