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

  driverTypeEl.addEventListener("change", () => {
    updateDriverVisibility();
  });

  clearButtonEls.forEach((buttonEl) => {
    buttonEl.addEventListener("click", reset);
  });
  sendButtonEls.forEach((buttonEl) => {
    buttonEl.addEventListener("click", renderPendingMessage);
  });

  updateDriverVisibility();
  reset();
})();
