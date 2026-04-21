#!/usr/bin/env python3
import fcntl
import json
import os
import sys
import time


I2C_SLAVE = 0x0703


def format_hex(values: list[int]) -> str:
    return " ".join(f"{value:02X}" for value in values)


def fail(message: str) -> None:
    print(json.dumps({"ok": False, "driver": "i2c", "error": message}))
    sys.exit(1)


def main() -> None:
    if len(sys.argv) != 2:
        fail("Expected one JSON payload argument.")

    try:
        payload = json.loads(sys.argv[1])
    except json.JSONDecodeError as exc:
        fail(f"Invalid JSON payload: {exc}")

    try:
        device_path = str(payload["devicePath"])
        address = int(payload["address"])
        speed = int(payload["speed"])
        read_length = int(payload["readLength"])
        timeout = int(payload["timeout"])
        tx_bytes = bytes(int(value) for value in payload["txBytes"])
    except (KeyError, TypeError, ValueError) as exc:
        fail(f"Invalid I2C payload: {exc}")

    try:
        fd = os.open(device_path, os.O_RDWR)
    except OSError as exc:
        fail(str(exc))

    try:
        fcntl.ioctl(fd, I2C_SLAVE, address)
        if tx_bytes:
            os.write(fd, tx_bytes)
        if timeout > 0:
            time.sleep(timeout / 1000)
        rx_bytes = os.read(fd, read_length) if read_length > 0 else b""
    except OSError as exc:
        fail(str(exc))
    finally:
        os.close(fd)

    rx_values = list(rx_bytes)
    print(
        json.dumps(
            {
                "ok": True,
                "driver": "i2c",
                "config": {
                    "devicePath": device_path,
                    "address": f"0x{address:02X}",
                    "addressValue": address,
                    "speed": speed,
                    "readLength": read_length,
                    "timeout": timeout,
                },
                "tx": {
                    "bytes": list(tx_bytes),
                    "hex": format_hex(list(tx_bytes)),
                    "length": len(tx_bytes),
                },
                "rx": {
                    "bytes": rx_values,
                    "hex": format_hex(rx_values),
                    "length": len(rx_values),
                },
            }
        )
    )


if __name__ == "__main__":
    main()
