#!/usr/bin/env python3
import grpc
import time
import pendulum_pb2
import pendulum_pb2_grpc

k_prop = 2.0
if __name__ == "__main__":
    channel = grpc.insecure_channel("pendulum-service:50051")
    stub = pendulum_pb2_grpc.pendulumStub(channel)
    reset_req = pendulum_pb2.request_reset(theta=1.0, omega=0.0)
    state = stub.reset(reset_req)
    print(
        f"start: "
        f"time={state.time:.2f}, "
        f"theta={state.theta:.4f}, "
        f"omega={state.omega:.4f}, "
        f"tau=0.0000",
        flush=True,
    )

    for i in range(2000):
        error = 0.0 - state.omega
        tau = k_prop * error
        step_req = pendulum_pb2.request_step(tau=tau)
        state = stub.step(step_req)
        time.sleep(0.05)
        print(
            f"step {i}: "
            f"time={state.time:.2f}, "
            f"theta={state.theta:.4f}, "
            f"tau={tau:.4f}",
            flush=True,
        )
