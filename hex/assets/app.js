const inputEl = document.getElementById("hex-input");
const parserTypeEl = document.getElementById("parser-type");
const resultTypeEl = document.getElementById("result-type");
const resultLengthEl = document.getElementById("result-length");
const resultBytesEl = document.getElementById("result-bytes");
const resultHexEl = document.getElementById("result-hex");
const resultParsedEl = document.getElementById("result-parsed");
const errorEl = document.getElementById("error");
let parseTimer = null;

function reset() {
  resultTypeEl.textContent = "-";
  resultLengthEl.textContent = "0";
  resultBytesEl.textContent = "[]";
  resultHexEl.textContent = "[]";
  resultParsedEl.textContent = "{}";
  errorEl.textContent = "";
}

async function parse() {
  errorEl.textContent = "";
  try {
    const response = await fetch("/api/hex/parse", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ input: inputEl.value, type: parserTypeEl.value }),
    });
    const data = await response.json();
    if (!response.ok) {
      throw new Error(data.error || "Parse failed");
    }
    resultTypeEl.textContent = String(data.type || "-");
    resultLengthEl.textContent = String(data.length ?? 0);
    resultBytesEl.textContent = JSON.stringify(data.bytes ?? [], null, 2);
    resultHexEl.textContent = JSON.stringify(data.hex ?? [], null, 2);
    resultParsedEl.textContent = JSON.stringify(data.parsed ?? {}, null, 2);
  } catch (error) {
    reset();
    errorEl.textContent = String(error && error.message ? error.message : error);
  }
}

function scheduleParse() {
  if (parseTimer) {
    clearTimeout(parseTimer);
  }
  parseTimer = setTimeout(parse, 120);
}

inputEl.addEventListener("input", scheduleParse);
parserTypeEl.addEventListener("change", scheduleParse);
reset();
parse();
