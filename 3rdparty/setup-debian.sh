#!/bin/bash
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