FROM ubuntu:24.04

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libx11-dev \
    libxrandr-dev \
    libxcursor-dev \
    libxi-dev \
    libfreetype6-dev \
    libglew-dev \
    libjpeg-dev \
    libopenal-dev \
    libflac-dev \
    libvorbis-dev \
    libudev-dev \
    && apt-get clean

WORKDIR /app

COPY . .
