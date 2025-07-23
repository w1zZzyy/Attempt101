FROM ghcr.io/userver-framework/ubuntu-24.04-userver:latest

ARG SERVICE_NAME

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libboost-all-dev \
    python3 \
    python3-pip \
    python3-venv \
    && apt-get clean

COPY ./run_as_user.sh /usr/local/bin/run_as_user.sh
RUN chmod +x /usr/local/bin/run_as_user.sh

WORKDIR /app
