#!/usr/bin/env python3
from datetime import datetime
from prometheus_client import start_http_server, Gauge
import os
import csv
import numpy
import docker
import grpc
import pendulum_pb2
import pendulum_pb2_grpc

def calc_energy(theta: float, omega: float):
    potential = 9.807E+0 * (1.000E+0 - numpy.cos(theta))
    kinetic = 5.000E-1 * omega**2
    return potential + kinetic

def calc_period(theta: float, time: float):
    if not hasattr(calc_period, "period"):
        calc_period.period = 0.000E+0
        calc_period.prev_theta = 0.000E+0
        calc_period.prev_cross_time = 0.000E+0

    if calc_period.prev_theta < 0.000E+0 and theta >= 0.000E+0:
        calc_period.period = time - calc_period.prev_cross_time
        calc_period.prev_cross_time = time

    calc_period.prev_theta = theta
    return calc_period.period

if __name__ == "__main__":
    start_http_server(8000)
    gauge_time = Gauge("simulation_time", "simulation_time")
    gauge_energy = Gauge("pendulum_energy", "pendulum_energy")
    gauge_period = Gauge("pendulum_period", "pendulum_period")
    channel = grpc.insecure_channel("pendulum-service:50051")
    stub = pendulum_pb2_grpc.pendulumStub(channel)
    reset_req = pendulum_pb2.request_reset(theta=1.0, omega=0.0)
    state = stub.reset(reset_req)
    print(
        f"start: "
        f"time={state.time:.2f}, "
        f"theta={state.theta:.4f}, "
        f"omega={state.omega:.4f}",
        flush=True,
    )

    time_prev = -1.0
    file_name = datetime.now().strftime("%Y%m%d_%H%M%S") + ".csv"
    file_path = os.path.join("/app/data", file_name)
    with open(file_path, "w", newline="") as file:
        writer = csv.writer(file)
        writer.writerow(["time", "theta", "omega"])
        try:
            for st in stub.stream_state(pendulum_pb2.empty()):
                if time_prev < st.time:
                    time_prev = st.time
                    if st.time >= 600.0:
                        print("target", flush=True)
                        break

                    gauge_time.set(st.time)
                    gauge_energy.set(calc_energy(st.theta, st.omega))
                    gauge_period.set(calc_period(st.theta, st.time))
                    writer.writerow([st.time, st.theta, st.omega])

        except grpc.RpcError as err:
            print("simulation error:", err, flush=True)

    print("terminate...", flush=True)
    client = docker.from_env()
    client.containers.get("control-service").stop()
    client.containers.get("pendulum-service").stop()
