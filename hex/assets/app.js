(() => {
  const { hasHexInput, sanitizeHexInput } = window.hexInputUtils;
  const inputEl = document.getElementById("hex-input");
  const parserTypeEl = document.getElementById("parser-type");
  const resultOutputEl = document.getElementById("result-output");
  let parseTimer = null;

  function formatPrimitive(value) {
    if (typeof value === "number") {
      return String(value);
    }
    if (typeof value === "boolean") return value ? "true" : "false";
    if (value === null) return "null";
    if (value === undefined) return "undefined";
    return String(value);
  }

  function formatArray(values) {
    if (values.length === 0) return "[]";
    if (values.every((v) => typeof v === "number")) {
      return values
        .map((v) => `0x${Number(v).toString(16).toUpperCase().padStart(2, "0")}`)
        .join(" ");
    }
    return values.map(formatPrimitive).join(", ");
  }

  function formatParsedValue(value) {
    if (Array.isArray(value)) {
      return formatArray(value);
    }
    if (value && typeof value === "object") {
      return JSON.stringify(value);
    }
    return formatPrimitive(value);
  }

  function formatParsedKey(key) {
    return String(key)
      .trim()
      .replace(/([a-z0-9])([A-Z])/g, "$1-$2")
      .replace(/[_\s]+/g, "-")
      .toLowerCase();
  }

  function renderParsed(parsed) {
    if (parsed === null || parsed === undefined) {
      resultOutputEl.textContent = String(parsed);
      return;
    }

    if (!parsed || typeof parsed !== "object") {
      resultOutputEl.textContent = formatPrimitive(parsed);
      return;
    }

    const entries = Object.entries(parsed);
    if (entries.length === 0) {
      resultOutputEl.textContent = "{}";
      return;
    }

    resultOutputEl.textContent = entries
      .map(([key, value]) => `${formatParsedKey(key)}: ${formatParsedValue(value)}`)
      .join("\n");
  }

  function reset() {
    resultOutputEl.textContent = "-";
  }

  async function requestParse() {
    const response = await fetch("/api/hex/parser", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ input: inputEl.value, type: parserTypeEl.value }),
    });
    const data = await response.json();
    if (!response.ok) {
      throw new Error(data.error || "Parse failed");
    }
    return data;
  }

  async function parse() {
    if (!hasHexInput(inputEl.value)) {
      reset();
      return;
    }

    try {
      const data = await requestParse();
      renderParsed(data.parsed);
    } catch (error) {
      resultOutputEl.textContent = String(error && error.message ? error.message : error);
    }
  }

  function scheduleParse() {
    if (parseTimer) {
      clearTimeout(parseTimer);
    }
    parseTimer = setTimeout(parse, 120);
  }

  ["input", "change"].forEach((eventName) => {
    inputEl.addEventListener(eventName, () => {
      const sanitized = sanitizeHexInput(inputEl.value);
      if (inputEl.value !== sanitized) {
        inputEl.value = sanitized;
      }

      scheduleParse();
    });
  });
  parserTypeEl.addEventListener("change", scheduleParse);
  reset();
})();
