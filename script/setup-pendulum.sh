#!/bin/bash
set -e
apt update
apt install -y \
        build-essential \
        meson \
        ninja-build \
        pkg-config \
        protobuf-compiler \
        libprotobuf-dev \
        libgrpc++-dev \
        libgrpc-dev \
        protobuf-compiler \
        protobuf-compiler-grpc \

cd /app/pendulum
meson setup build || true
meson compile -C build