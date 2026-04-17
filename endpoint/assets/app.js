(() => {
  const inputEl = document.getElementById("endpoint-hex-input");
  const driverTypeEl = document.getElementById("endpoint-driver-type");
  const resultOutputEl = document.getElementById("endpoint-result-output");
  const clearButtonEl = document.getElementById("endpoint-clear-button");
  const sendButtonEl = document.getElementById("endpoint-send-button");
  const clearMirrorEls = Array.from(document.querySelectorAll('[data-role="endpoint-clear-mirror"]'));
  const sendMirrorEls = Array.from(document.querySelectorAll('[data-role="endpoint-send-mirror"]'));
  const settingFieldEls = Array.from(document.querySelectorAll(".endpoint-panel [data-driver]"));

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

  clearButtonEl.addEventListener("click", reset);
  sendButtonEl.addEventListener("click", renderPendingMessage);
  clearMirrorEls.forEach((buttonEl) => {
    buttonEl.addEventListener("click", reset);
  });
  sendMirrorEls.forEach((buttonEl) => {
    buttonEl.addEventListener("click", renderPendingMessage);
  });

  updateDriverVisibility();
  reset();
})();
