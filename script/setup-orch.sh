#!/bin/bash
pip install \
        docker \
        grpcio \
        grpcio-tools \
        prometheus-client \

python -m grpc_tools.protoc \
        -I/app/orch/proto \
        --python_out=/app/orch \
        --grpc_python_out=/app/orch \
        /app/orch/proto/pendulum.proto