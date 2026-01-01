#!/usr/bin/env python3
import grpc
import time
import pendulum_pb2
import pendulum_pb2_grpc

k_prop = 2.0
tau = 0.0
if __name__ == "__main__":
    channel = grpc.insecure_channel("pendulum-service:50051")
    stub = pendulum_pb2_grpc.pendulumStub(channel)
    while True:
        step_req = pendulum_pb2.request_step(tau=tau)
        state = stub.step(step_req)
        tau = -k_prop * state.omega
        time.sleep(0.05)
