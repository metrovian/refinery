#!/usr/bin/env python3
import grpc
import time
import numpy
import pendulum_pb2
import pendulum_pb2_grpc

def calc_energy(theta: float, omega: float):
    potential = 9.807E+0 * (1.000E+0 - numpy.cos(theta))
    kinetic = 5.000E-1 * omega**2
    return potential + kinetic

def calc_torque(theta: float, omega: float):
    delta = target - calc_energy(theta, omega)
    if delta > 0: 
        return 1.000E+2 * delta * numpy.sign(omega)
    else: 
        return 0

if __name__ == "__main__":
    channel = grpc.insecure_channel("pendulum-service:50051")
    stub = pendulum_pb2_grpc.pendulumStub(channel)
    tau = 0
    target = calc_energy(1.000E+0, 0.000E+0)
    while True:
        step_req = pendulum_pb2.request_step(tau=tau)
        state = stub.step(step_req)
        tau = calc_torque(state.theta, state.omega)
        time.sleep(0.05)
