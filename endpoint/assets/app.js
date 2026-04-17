const inputEl = document.getElementById("hex-input");
const driverTypeEl = document.getElementById("driver-type");
const resultOutputEl = document.getElementById("result-output");
const clearButtonEl = document.getElementById("clear-button");
const sendButtonEl = document.getElementById("send-button");
const clearMirrorEls = Array.from(document.querySelectorAll('[data-role="clear-mirror"]'));
const sendMirrorEls = Array.from(document.querySelectorAll('[data-role="send-mirror"]'));
const settingFieldEls = Array.from(document.querySelectorAll("[data-driver]"));

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
