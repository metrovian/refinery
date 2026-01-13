#!/usr/bin/env python3
from datetime import datetime
from prometheus_client import start_http_server, Gauge, Counter
import os
import csv
import time
import docker
import grpc
import pendulum_pb2
import pendulum_pb2_grpc

if __name__ == "__main__":
    start_http_server(8000)
    simulation_time = Gauge("simulation_time", "simulation_time")
    simulation_start_total = Counter("simulation_start_total", "simulation_start_total")
    simulation_stop_total = Counter("simulation_stop_total", "simulation_stop_total")
    simulation_start_total.inc()
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
                    if st.time >= 10.0:
                        print("target", flush=True)
                        break

                    simulation_time.set(st.time)
                    writer.writerow([st.time, st.theta, st.omega])
                    print(
                        f"time={st.time:.2f}, "
                        f"theta={st.theta:.4f}, "
                        f"omega={st.omega:.4f}",
                        flush=True,
                    )

        except grpc.RpcError as err:
            print("simulation error:", err, flush=True)

    print("terminate...", flush=True)
    client = docker.from_env()
    client.containers.get("control-service").stop()
    client.containers.get("pendulum-service").stop()
    simulation_stop_total.inc()
