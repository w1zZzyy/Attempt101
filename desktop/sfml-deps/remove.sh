#!/bin/bash

echo "Removing SFML dependencies..."
sudo apt remove --purge -y \
    libx11-dev libxrandr-dev libxcursor-dev libxi-dev libxext-dev \
    libxinerama-dev libxfixes-dev libxrender-dev \
    libgl1-mesa-dev mesa-common-dev libudev-dev \
    libfreetype6-dev libharfbuzz-dev

echo "Removing unused packages..."
sudo apt autoremove -y

echo "SFML dependencies removed!"
