#!/bin/bash
set -e
cd /app/pendulum
meson setup build || true
meson compile -C build