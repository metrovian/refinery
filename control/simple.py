#!/usr/bin/env python3
import grpc
import time
import numpy
import pendulum_pb2
import pendulum_pb2_grpc

def calc_pendulum_energy(theta: float, omega: float):
    potential = 9.807E+0 * (1.000E+0 - numpy.cos(theta))
    kinetic = 5.000E-1 * omega**2
    return potential + kinetic

if __name__ == "__main__":
    channel = grpc.insecure_channel("pendulum-service:50051")
    stub = pendulum_pb2_grpc.pendulumStub(channel)
    tau = 0
    target = calc_pendulum_energy(1.000E+0, 0.000E+0)
    while True:
        step_req = pendulum_pb2.request_step(tau=tau)
        state = stub.step(step_req)
        delta = target - calc_pendulum_energy(state.theta, state.omega)
        if delta > 0:
            tau = delta * numpy.sign(state.omega)
        
        time.sleep(0.05)
