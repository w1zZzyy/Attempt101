#!/bin/bash

echo "Updating packages ..."
sudo apt update

echo "Downloading SFML dependencies ..."    
sudo apt install -y \
    libx11-dev libxrandr-dev libxcursor-dev libxi-dev libxext-dev \
    libxinerama-dev libxfixes-dev libxrender-dev \
    libgl1-mesa-dev mesa-common-dev  libudev-dev \
    libfreetype6-dev libharfbuzz-dev

echo "SFML dependencies downloaded!"