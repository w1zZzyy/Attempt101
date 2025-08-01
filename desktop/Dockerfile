FROM ubuntu:24.04

RUN apt-get update && apt-get install -y \
    g++ \
    cmake \
    git \
    gdb \
    libx11-dev \
    libxrandr-dev \
    libfreetype6-dev \
    libglew-dev \
    libjpeg-dev \
    libopenal-dev \
    libflac-dev \
    libvorbis-dev \
    libudev-dev \
    && apt-get clean

WORKDIR /app

COPY desktop/ .

COPY libs/ ../libs/
