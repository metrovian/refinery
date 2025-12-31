#!/bin/bash
pip install \
        grpcio \
        grpcio-tools \

python -m grpc_tools.protoc \
        -I/app/control/proto \
        --python_out=/app/control \
        --grpc_python_out=/app/control \
        /app/control/proto/pendulum.proto