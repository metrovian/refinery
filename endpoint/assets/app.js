(() => {
  const inputEl = document.getElementById("endpoint-hex-input");
  const driverTypeEl = document.getElementById("endpoint-driver-type");
  const resultOutputEl = document.getElementById("endpoint-result-output");
  const settingFieldEls = Array.from(document.querySelectorAll(".endpoint-panel [data-driver]"));
  const clearButtonEls = [
    document.getElementById("endpoint-clear-button"),
    ...document.querySelectorAll('[data-role="endpoint-clear-mirror"]'),
  ].filter(Boolean);
  const sendButtonEls = [
    document.getElementById("endpoint-send-button"),
    ...document.querySelectorAll('[data-role="endpoint-send-mirror"]'),
  ].filter(Boolean);

  function sanitizeHexInput(value) {
    return value
      .toUpperCase()
      .replace(/[^0-9A-FX\s,;:-]/g, "")
      .replace(/[,:;-]+/g, " ")
      .replace(/\s+/g, " ")
      .trimStart();
  }

  function hasHexInput() {
    return inputEl.value.trim().length > 0;
  }

  function getFieldValue(driver, key) {
    const fieldEl = document.getElementById(`endpoint-${driver}-${key}`);
    return fieldEl ? fieldEl.value : "";
  }

  function renderLines(lines) {
    resultOutputEl.textContent = lines.join("\n");
  }

  function setButtonsDisabled(disabled) {
    sendButtonEls.forEach((buttonEl) => {
      buttonEl.disabled = disabled;
    });
  }

  function updateDriverVisibility() {
    const currentDriver = driverTypeEl.value;
    settingFieldEls.forEach((fieldEl) => {
      fieldEl.hidden = fieldEl.dataset.driver !== currentDriver;
    });
  }

  function reset() {
    inputEl.value = "";
    resultOutputEl.textContent = "-";
  }

  function renderPendingMessage() {
    resultOutputEl.textContent = [
      `driver: ${driverTypeEl.value}`,
      "status: not-implemented",
      "message: transport logic is not connected yet",
    ].join("\n");
  }

  function renderUartResult(result) {
    renderLines([
      `driver: ${result.driver}`,
      `device: ${result.config.devicePath}`,
      `baud: ${result.config.baudRate}`,
      `format: ${result.config.dataBits}${result.config.parity[0].toUpperCase()}${result.config.stopBits}`,
      `timeout: ${result.config.timeout} ms`,
      `tx (${result.tx.length}): ${result.tx.hex || "-"}`,
      `rx (${result.rx.length}): ${result.rx.hex || "(no response)"}`,
    ]);
  }

  function renderSpiResult(result) {
    renderLines([
      `driver: ${result.driver}`,
      `device: ${result.config.devicePath}`,
      `speed: ${result.config.speed} Hz`,
      `mode: ${result.config.mode}`,
      `bits: ${result.config.bitsPerWord}`,
      `delay: ${result.config.delay} us`,
      `tx (${result.tx.length}): ${result.tx.hex || "-"}`,
      `rx (${result.rx.length}): ${result.rx.hex || "(no response)"}`,
    ]);
  }

  function renderI2cResult(result) {
    renderLines([
      `driver: ${result.driver}`,
      `device: ${result.config.devicePath}`,
      `address: ${result.config.address}`,
      `speed: ${result.config.speed} Hz`,
      `read length: ${result.config.readLength}`,
      `timeout: ${result.config.timeout} ms`,
      `tx (${result.tx.length}): ${result.tx.hex || "-"}`,
      `rx (${result.rx.length}): ${result.rx.hex || "(no response)"}`,
    ]);
  }

  async function sendUart() {
    if (!hasHexInput()) {
      resultOutputEl.textContent = "-";
      return;
    }

    const payload = {
      input: inputEl.value,
      devicePath: getFieldValue("uart", "device-path"),
      baudRate: Number(getFieldValue("uart", "baud-rate")),
      dataBits: Number(getFieldValue("uart", "data-bits")),
      parity: getFieldValue("uart", "parity"),
      stopBits: Number(getFieldValue("uart", "stop-bits")),
      timeout: Number(getFieldValue("uart", "timeout")),
    };

    setButtonsDisabled(true);
    renderLines([
      "driver: uart",
      "status: pending",
      `device: ${payload.devicePath || "/dev/serial0"}`,
    ]);

    try {
      const response = await fetch("/api/endpoint/uart/send", {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify(payload),
      });
      const result = await response.json();

      if (!response.ok || !result.ok) {
        throw new Error(result.error || "UART transfer failed.");
      }

      renderUartResult(result);
    } catch (error) {
      renderLines([
        "driver: uart",
        "status: failed",
        `error: ${error instanceof Error ? error.message : "Unknown error"}`,
      ]);
    } finally {
      setButtonsDisabled(false);
    }
  }

  async function sendSpi() {
    if (!hasHexInput()) {
      resultOutputEl.textContent = "-";
      return;
    }

    const payload = {
      input: inputEl.value,
      devicePath: getFieldValue("spi", "device-path"),
      speed: Number(getFieldValue("spi", "speed")),
      mode: Number(getFieldValue("spi", "mode")),
      bitsPerWord: Number(getFieldValue("spi", "bits-per-word")),
      delay: Number(getFieldValue("spi", "delay")),
    };

    setButtonsDisabled(true);
    renderLines([
      "driver: spi",
      "status: pending",
      `device: ${payload.devicePath || "/dev/spidev0.0"}`,
    ]);

    try {
      const response = await fetch("/api/endpoint/spi/send", {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify(payload),
      });
      const result = await response.json();

      if (!response.ok || !result.ok) {
        throw new Error(result.error || "SPI transfer failed.");
      }

      renderSpiResult(result);
    } catch (error) {
      renderLines([
        "driver: spi",
        "status: failed",
        `error: ${error instanceof Error ? error.message : "Unknown error"}`,
      ]);
    } finally {
      setButtonsDisabled(false);
    }
  }

  async function sendI2c() {
    if (!hasHexInput()) {
      resultOutputEl.textContent = "-";
      return;
    }

    const payload = {
      input: inputEl.value,
      devicePath: getFieldValue("i2c", "device-path"),
      address: getFieldValue("i2c", "address"),
      speed: Number(getFieldValue("i2c", "speed")),
      readLength: Number(getFieldValue("i2c", "read-length")),
      timeout: Number(getFieldValue("i2c", "timeout")),
    };

    setButtonsDisabled(true);
    renderLines([
      "driver: i2c",
      "status: pending",
      `device: ${payload.devicePath || "/dev/i2c-1"}`,
      `address: ${payload.address || "-"}`,
    ]);

    try {
      const response = await fetch("/api/endpoint/i2c/send", {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify(payload),
      });
      const result = await response.json();

      if (!response.ok || !result.ok) {
        throw new Error(result.error || "I2C transfer failed.");
      }

      renderI2cResult(result);
    } catch (error) {
      renderLines([
        "driver: i2c",
        "status: failed",
        `error: ${error instanceof Error ? error.message : "Unknown error"}`,
      ]);
    } finally {
      setButtonsDisabled(false);
    }
  }

  driverTypeEl.addEventListener("change", () => {
    updateDriverVisibility();
  });
  inputEl.addEventListener("input", () => {
    const sanitized = sanitizeHexInput(inputEl.value);
    if (inputEl.value !== sanitized) {
      inputEl.value = sanitized;
    }

    if (!hasHexInput()) {
      resultOutputEl.textContent = "-";
    }
  });

  clearButtonEls.forEach((buttonEl) => {
    buttonEl.addEventListener("click", reset);
  });
  sendButtonEls.forEach((buttonEl) => {
    buttonEl.addEventListener("click", () => {
      if (driverTypeEl.value === "uart") {
        void sendUart();
        return;
      }

      if (driverTypeEl.value === "spi") {
        void sendSpi();
        return;
      }

      if (driverTypeEl.value === "i2c") {
        void sendI2c();
        return;
      }

      renderPendingMessage();
    });
  });

  updateDriverVisibility();
  reset();
})();
