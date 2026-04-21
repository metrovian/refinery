#!/usr/bin/env python3
import ctypes
import fcntl
import json
import os
import struct
import sys


IOC_NRBITS = 8
IOC_TYPEBITS = 8
IOC_SIZEBITS = 14
IOC_DIRBITS = 2

IOC_NRSHIFT = 0
IOC_TYPESHIFT = IOC_NRSHIFT + IOC_NRBITS
IOC_SIZESHIFT = IOC_TYPESHIFT + IOC_TYPEBITS
IOC_DIRSHIFT = IOC_SIZESHIFT + IOC_SIZEBITS

IOC_NONE = 0
IOC_WRITE = 1

SPI_IOC_MAGIC = ord("k")


def ioc(direction: int, spi_type: int, number: int, size: int) -> int:
    return (
        (direction << IOC_DIRSHIFT)
        | (spi_type << IOC_TYPESHIFT)
        | (number << IOC_NRSHIFT)
        | (size << IOC_SIZESHIFT)
    )


def iow(spi_type: int, number: int, size: int) -> int:
    return ioc(IOC_WRITE, spi_type, number, size)


SPI_IOC_WR_MODE = iow(SPI_IOC_MAGIC, 1, 1)
SPI_IOC_WR_BITS_PER_WORD = iow(SPI_IOC_MAGIC, 3, 1)
SPI_IOC_WR_MAX_SPEED_HZ = iow(SPI_IOC_MAGIC, 4, 4)
SPI_IOC_MESSAGE_1 = iow(SPI_IOC_MAGIC, 0, 32)


def format_hex(values: list[int]) -> str:
    return " ".join(f"{value:02X}" for value in values)


def fail(message: str) -> None:
    print(json.dumps({"ok": False, "driver": "spi", "error": message}))
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
        speed = int(payload["speed"])
        mode = int(payload["mode"])
        bits_per_word = int(payload["bitsPerWord"])
        delay = int(payload["delay"])
        tx_bytes = [int(value) for value in payload["txBytes"]]
    except (KeyError, TypeError, ValueError) as exc:
        fail(f"Invalid SPI payload: {exc}")

    tx_buffer = (ctypes.c_ubyte * len(tx_bytes))(*tx_bytes)
    rx_buffer = (ctypes.c_ubyte * len(tx_bytes))()
    transfer = struct.pack(
        "<QQIIH6B",
        ctypes.addressof(tx_buffer),
        ctypes.addressof(rx_buffer),
        len(tx_bytes),
        speed,
        delay,
        bits_per_word,
        0,
        0,
        0,
        0,
        0,
    )
    transfer_buffer = bytearray(transfer)

    try:
        fd = os.open(device_path, os.O_RDWR)
    except OSError as exc:
        fail(str(exc))

    try:
        fcntl.ioctl(fd, SPI_IOC_WR_MODE, struct.pack("B", mode))
        fcntl.ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, struct.pack("B", bits_per_word))
        fcntl.ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, struct.pack("I", speed))
        fcntl.ioctl(fd, SPI_IOC_MESSAGE_1, transfer_buffer, True)
    except OSError as exc:
        fail(str(exc))
    finally:
        os.close(fd)

    rx_values = [int(value) for value in rx_buffer]
    print(
        json.dumps(
            {
                "ok": True,
                "driver": "spi",
                "config": {
                    "devicePath": device_path,
                    "speed": speed,
                    "mode": mode,
                    "bitsPerWord": bits_per_word,
                    "delay": delay,
                },
                "tx": {
                    "bytes": tx_bytes,
                    "hex": format_hex(tx_bytes),
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
