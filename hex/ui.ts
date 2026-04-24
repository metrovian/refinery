import { listParsers } from "./registry";

function renderParserOptions(): string {
  return listParsers()
    .map(({ type, label }) => `<option value="${type}">${label}</option>`)
    .join("\n");
}

export function renderHexPanel(): string {
  return `<section class="page-panel hex-panel">
    <div class="input-box">
      <div class="control-row">
        <div class="select-wrap">
          <select id="parser-type" class="top-select">
            ${renderParserOptions()}
          </select>
        </div>
      </div>
      <pre id="result-output" class="parsed-output">-</pre>
    </div>
    <textarea id="hex-input" class="raw-input" spellcheck="false" placeholder="INPUT HEX"></textarea>
  </section>`;
}
