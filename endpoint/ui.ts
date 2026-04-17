export function renderEndpointPanel(): string {
  return `<section class="page-panel endpoint-panel">
    <div class="input-box">
      <div class="control-row">
        <div class="select-wrap">
          <select id="endpoint-driver-type" class="top-select">
            <option value="uart">uart</option>
            <option value="spi">spi</option>
            <option value="i2c">i2c</option>
          </select>
        </div>
      </div>
      <div class="settings-grid settings-inline">
        <label class="setting-field" data-driver="uart">
          <span class="setting-label">BAUD RATE</span>
          <input id="endpoint-uart-baud-rate" type="number" value="115200" />
        </label>
        <label class="setting-field" data-driver="uart">
          <span class="setting-label">DATA BITS</span>
          <select id="endpoint-uart-data-bits">
            <option value="8" selected>8</option>
            <option value="7">7</option>
          </select>
        </label>
        <label class="setting-field" data-driver="uart">
          <span class="setting-label">PARITY</span>
          <select id="endpoint-uart-parity">
            <option value="none" selected>none</option>
            <option value="even">even</option>
            <option value="odd">odd</option>
          </select>
        </label>
        <label class="setting-field" data-driver="uart">
          <span class="setting-label">STOP BITS</span>
          <select id="endpoint-uart-stop-bits">
            <option value="1" selected>1</option>
            <option value="2">2</option>
          </select>
        </label>
        <label class="setting-field setting-field-actions" data-driver="uart">
          <span class="setting-label">TIMEOUT</span>
          <div class="setting-value-row">
            <input id="endpoint-uart-timeout" type="number" value="1000" />
            <div class="action-row">
              <button id="endpoint-clear-button" type="button" class="action-button secondary">clear</button>
              <button id="endpoint-send-button" type="button" class="action-button">send</button>
            </div>
          </div>
        </label>

        <label class="setting-field" data-driver="spi" hidden>
          <span class="setting-label">SPEED</span>
          <input id="endpoint-spi-speed" type="number" value="1000000" />
        </label>
        <label class="setting-field" data-driver="spi" hidden>
          <span class="setting-label">MODE</span>
          <select id="endpoint-spi-mode">
            <option value="0" selected>mode 0</option>
            <option value="1">mode 1</option>
            <option value="2">mode 2</option>
            <option value="3">mode 3</option>
          </select>
        </label>
        <label class="setting-field" data-driver="spi" hidden>
          <span class="setting-label">BITS</span>
          <input id="endpoint-spi-bits-per-word" type="number" value="8" />
        </label>
        <label class="setting-field setting-field-actions" data-driver="spi" hidden>
          <span class="setting-label">CHIP SELECT</span>
          <div class="setting-value-row">
            <input id="endpoint-spi-chip-select" type="text" value="CS0" />
            <div class="action-row">
              <button type="button" class="action-button secondary" data-role="endpoint-clear-mirror">clear</button>
              <button type="button" class="action-button" data-role="endpoint-send-mirror">send</button>
            </div>
          </div>
        </label>

        <label class="setting-field" data-driver="i2c" hidden>
          <span class="setting-label">ADDRESS</span>
          <input id="endpoint-i2c-address" type="text" value="0x3C" />
        </label>
        <label class="setting-field" data-driver="i2c" hidden>
          <span class="setting-label">SPEED</span>
          <input id="endpoint-i2c-speed" type="number" value="400000" />
        </label>
        <label class="setting-field setting-field-actions" data-driver="i2c" hidden>
          <span class="setting-label">TIMEOUT</span>
          <div class="setting-value-row">
            <input id="endpoint-i2c-timeout" type="number" value="1000" />
            <div class="action-row">
              <button type="button" class="action-button secondary" data-role="endpoint-clear-mirror">clear</button>
              <button type="button" class="action-button" data-role="endpoint-send-mirror">send</button>
            </div>
          </div>
        </label>
      </div>
      <pre id="endpoint-result-output" class="parsed-output">-</pre>
    </div>
    <textarea
      id="endpoint-hex-input"
      class="raw-input"
      spellcheck="false"
      placeholder="0A FF 1C"
    ></textarea>
  </section>`;
}
