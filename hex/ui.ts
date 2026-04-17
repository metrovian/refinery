export function renderHexPanel(): string {
  return `<section class="page-panel hex-panel">
    <div class="input-box">
      <div class="control-row">
        <div class="select-wrap">
          <select id="parser-type" class="top-select">
            <option value="modbus-rtu">MODBUS-RTU</option>
            <!-- [PARSER:UI_OPTION] -->
          </select>
        </div>
      </div>
      <pre id="result-output" class="parsed-output">-</pre>
    </div>
    <textarea id="hex-input" class="raw-input" spellcheck="false" placeholder="0A FF 1C"></textarea>
  </section>`;
}
