FROM ubuntu:24.04

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libxrandr-dev \
    libxcursor-dev \
    libxi-dev \
    libudev-dev \
    libflac-dev \
    libvorbis-dev \
    libgl1-mesa-dev \
    libegl1-mesa-dev \
    libdrm-dev \
    libgbm-dev \
    libfreetype6-dev \
    pkg-config \
    && apt-get clean

WORKDIR /app

COPY ./libs ./libs
COPY ./desktop ./desktop
COPY ./Makefile ./Makefile 
COPY ./CMakeLists.txt ./CMakeLists.txt 
