(() => {
  const { hasHexInput, sanitizeHexInput } = window.hexInputUtils;
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

  function getFieldValue(driver, key) {
    const fieldEl = document.getElementById(`endpoint-${driver}-${key}`);
    return fieldEl ? fieldEl.value : "";
  }

  function getNumberFieldValue(driver, key) {
    return Number(getFieldValue(driver, key));
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

  function renderTransferResult(details, tx, rx) {
    renderLines([
      ...details,
      `tx[${tx.length}]: ${tx.hex || "-"}`,
      `rx[${rx.length}]: ${rx.hex || "(no response)"}`,
    ]);
  }

  function renderPending(details) {
    renderLines(["status: pending", ...details]);
  }

  function createDriverConfig(spec) {
    return {
      endpoint: spec.endpoint,
      errorMessage: spec.errorMessage,
      buildPayload: spec.buildPayload,
      renderPending(payload) {
        renderPending(spec.pendingLines(payload));
      },
      renderResult(result) {
        renderTransferResult(spec.resultLines(result), result.tx, result.rx);
      },
    };
  }

  const driverConfigs = {
    uart: createDriverConfig({
      endpoint: "/api/endpoint/uart/send",
      errorMessage: "UART transfer failed.",
      buildPayload() {
        return {
          input: inputEl.value,
          devicePath: getFieldValue("uart", "device-path"),
          baudRate: getNumberFieldValue("uart", "baud-rate"),
          dataBits: getNumberFieldValue("uart", "data-bits"),
          parity: getFieldValue("uart", "parity"),
          stopBits: getNumberFieldValue("uart", "stop-bits"),
          timeout: getNumberFieldValue("uart", "timeout"),
        };
      },
      pendingLines(payload) {
        return [`device: ${payload.devicePath || "/dev/serial0"}`];
      },
      resultLines(result) {
        return [
          `device: ${result.config.devicePath}`,
          `set: ${result.config.dataBits}${result.config.parity[0].toUpperCase()}${result.config.stopBits}`,
          `baud: ${result.config.baudRate} bps`,
          `timeout: ${result.config.timeout} ms`,
        ];
      },
    }),
    spi: createDriverConfig({
      endpoint: "/api/endpoint/spi/send",
      errorMessage: "SPI transfer failed.",
      buildPayload() {
        return {
          input: inputEl.value,
          devicePath: getFieldValue("spi", "device-path"),
          speed: getNumberFieldValue("spi", "speed"),
          mode: getNumberFieldValue("spi", "mode"),
          bitsPerWord: getNumberFieldValue("spi", "bits-per-word"),
          delay: getNumberFieldValue("spi", "delay"),
        };
      },
      pendingLines(payload) {
        return [`device: ${payload.devicePath || "/dev/spidev0.0"}`];
      },
      resultLines(result) {
        return [
          `device: ${result.config.devicePath}`,
          `speed: ${result.config.speed} Hz`,
          `mode: ${result.config.mode}`,
          `bits: ${result.config.bitsPerWord}`,
          `delay: ${result.config.delay} us`,
        ];
      },
    }),
    i2c: createDriverConfig({
      endpoint: "/api/endpoint/i2c/send",
      errorMessage: "I2C transfer failed.",
      buildPayload() {
        return {
          input: inputEl.value,
          devicePath: getFieldValue("i2c", "device-path"),
          address: getFieldValue("i2c", "address"),
          speed: getNumberFieldValue("i2c", "speed"),
          readLength: getNumberFieldValue("i2c", "read-length"),
          timeout: getNumberFieldValue("i2c", "timeout"),
        };
      },
      pendingLines(payload) {
        return [
          `device: ${payload.devicePath || "/dev/i2c-1"}`,
          `address: ${payload.address || "-"}`,
        ];
      },
      resultLines(result) {
        return [
          `device: ${result.config.devicePath}`,
          `address: ${result.config.address}`,
          `speed: ${result.config.speed} Hz`,
          `timeout: ${result.config.timeout} ms`,
        ];
      },
    }),
  };

  async function sendDriverRequest(driver) {
    if (!hasHexInput(inputEl.value)) {
      resultOutputEl.textContent = "-";
      return;
    }

    const config = driverConfigs[driver];
    const payload = config.buildPayload();

    setButtonsDisabled(true);
    config.renderPending(payload);

    try {
      const response = await fetch(config.endpoint, {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify(payload),
      });
      const result = await response.json();

      if (!response.ok || !result.ok) {
        throw new Error(result.error || config.errorMessage);
      }

      config.renderResult(result);
    } catch (error) {
      renderLines([
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

    if (!hasHexInput(inputEl.value)) {
      resultOutputEl.textContent = "-";
    }
  });

  clearButtonEls.forEach((buttonEl) => {
    buttonEl.addEventListener("click", reset);
  });
  sendButtonEls.forEach((buttonEl) => {
    buttonEl.addEventListener("click", () => {
      if (driverTypeEl.value in driverConfigs) {
        void sendDriverRequest(driverTypeEl.value);
      }
    });
  });

  updateDriverVisibility();
  reset();
})();
