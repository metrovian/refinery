const samples = {
  can: "18FEF100 01 0A FF 1C",
  modbus: "01 03 00 6B 00 03 76 87",
  raw: "0A FF 1C",
};

const inputEl = document.getElementById("hex-input");
const parserTypeEl = document.getElementById("parser-type");
const parseBtn = document.getElementById("parse-btn");
const clearBtn = document.getElementById("clear-btn");
const tokensEl = document.getElementById("tokens");
const summaryEl = document.getElementById("summary");
const lengthEl = document.getElementById("length");
const bytesEl = document.getElementById("bytes");
const hexEl = document.getElementById("hex");
const parsedEl = document.getElementById("parsed");
const errorEl = document.getElementById("error");

const tokenize = (value) => value.trim().split(/[\s,;:-]+/).filter(Boolean);

const valid = (token) => {
  const normalized = token.replace(/^0x/i, "");
  return !!normalized && /^[0-9a-fA-F]+$/.test(normalized) &&
    (normalized.length <= 2 || normalized.length % 2 === 0);
};

function renderValidation() {
  const tokens = tokenize(inputEl.value);
  tokensEl.innerHTML = "";
  if (!tokens.length) {
    summaryEl.textContent = "입력 대기 중";
    return;
  }

  let invalidCount = 0;
  tokens.forEach((token) => {
    const ok = valid(token);
    if (!ok) invalidCount += 1;
    const el = document.createElement("span");
    el.className = `token ${ok ? "ok" : "bad"}`;
    el.textContent = token;
    tokensEl.appendChild(el);
  });

  summaryEl.textContent = invalidCount === 0
    ? `정상 토큰 ${tokens.length}개`
    : `오류 토큰 ${invalidCount}개 / 전체 ${tokens.length}개`;
}

function reset() {
  lengthEl.textContent = "0";
  bytesEl.textContent = "[]";
  hexEl.textContent = "[]";
  parsedEl.textContent = "{}";
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
    lengthEl.textContent = String(data.length ?? 0);
    bytesEl.textContent = JSON.stringify(data.bytes ?? [], null, 2);
    hexEl.textContent = JSON.stringify(data.hex ?? [], null, 2);
    parsedEl.textContent = JSON.stringify(data.parsed ?? {}, null, 2);
  } catch (error) {
    reset();
    errorEl.textContent = String(error && error.message ? error.message : error);
  }
}

document.querySelectorAll("[data-sample]").forEach((button) => {
  button.addEventListener("click", () => {
    const key = button.getAttribute("data-sample");
    parserTypeEl.value = key || "raw";
    inputEl.value = samples[key] || "";
    renderValidation();
    parse();
  });
});

parseBtn.addEventListener("click", parse);
clearBtn.addEventListener("click", () => {
  parserTypeEl.value = "raw";
  inputEl.value = "";
  tokensEl.innerHTML = "";
  summaryEl.textContent = "입력 대기 중";
  reset();
});

inputEl.addEventListener("input", renderValidation);
renderValidation();
reset();
