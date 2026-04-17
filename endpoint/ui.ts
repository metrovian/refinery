export function renderEndpointUiPage(): string {
  return `<!doctype html>
<html lang="ko">
  <head>
    <meta charset="utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <link rel="stylesheet" href="/endpoint/assets/styles.css" />
  </head>
  <body>
    <main class="layout">
      <section class="panel">
        <div class="input-box">
          <div class="control-row">
            <div class="select-wrap">
              <select id="driver-type">
                <option value="uart">uart</option>
                <option value="spi">spi</option>
                <option value="i2c">i2c</option>
              </select>
            </div>
          </div>
          <div class="settings-grid settings-inline">
            <label class="setting-field" data-driver="uart">
              <span class="setting-label">BAUD RATE</span>
              <input id="uart-baud-rate" type="number" value="115200" />
            </label>
            <label class="setting-field" data-driver="uart">
              <span class="setting-label">DATA BITS</span>
              <select id="uart-data-bits">
                <option value="8" selected>8</option>
                <option value="7">7</option>
              </select>
            </label>
            <label class="setting-field" data-driver="uart">
              <span class="setting-label">PARITY</span>
              <select id="uart-parity">
                <option value="none" selected>none</option>
                <option value="even">even</option>
                <option value="odd">odd</option>
              </select>
            </label>
            <label class="setting-field" data-driver="uart">
              <span class="setting-label">STOP BITS</span>
              <select id="uart-stop-bits">
                <option value="1" selected>1</option>
                <option value="2">2</option>
              </select>
            </label>
            <label class="setting-field setting-field-actions" data-driver="uart">
              <span class="setting-label">TIMEOUT</span>
              <div class="setting-value-row">
                <input id="uart-timeout" type="number" value="1000" />
                <div class="action-row">
                  <button id="clear-button" type="button" class="action-button secondary">clear</button>
                  <button id="send-button" type="button" class="action-button">send</button>
                </div>
              </div>
            </label>

            <label class="setting-field" data-driver="spi" hidden>
              <span class="setting-label">SPEED</span>
              <input id="spi-speed" type="number" value="1000000" />
            </label>
            <label class="setting-field" data-driver="spi" hidden>
              <span class="setting-label">MODE</span>
              <select id="spi-mode">
                <option value="0" selected>mode 0</option>
                <option value="1">mode 1</option>
                <option value="2">mode 2</option>
                <option value="3">mode 3</option>
              </select>
            </label>
            <label class="setting-field" data-driver="spi" hidden>
              <span class="setting-label">BITS</span>
              <input id="spi-bits-per-word" type="number" value="8" />
            </label>
            <label class="setting-field setting-field-actions" data-driver="spi" hidden>
              <span class="setting-label">CHIP SELECT</span>
              <div class="setting-value-row">
                <input id="spi-chip-select" type="text" value="CS0" />
                <div class="action-row">
                  <button type="button" class="action-button secondary" data-role="clear-mirror">clear</button>
                  <button type="button" class="action-button" data-role="send-mirror">send</button>
                </div>
              </div>
            </label>

            <label class="setting-field" data-driver="i2c" hidden>
              <span class="setting-label">ADDRESS</span>
              <input id="i2c-address" type="text" value="0x3C" />
            </label>
            <label class="setting-field" data-driver="i2c" hidden>
              <span class="setting-label">SPEED</span>
              <input id="i2c-speed" type="number" value="400000" />
            </label>
            <label class="setting-field setting-field-actions" data-driver="i2c" hidden>
              <span class="setting-label">TIMEOUT</span>
              <div class="setting-value-row">
                <input id="i2c-timeout" type="number" value="1000" />
                <div class="action-row">
                  <button type="button" class="action-button secondary" data-role="clear-mirror">clear</button>
                  <button type="button" class="action-button" data-role="send-mirror">send</button>
                </div>
              </div>
            </label>
          </div>
          <pre id="result-output" class="parsed-output">-</pre>
        </div>
        <textarea
          id="hex-input"
          class="raw-input"
          spellcheck="false"
          placeholder="0A FF 1C"
        ></textarea>
      </section>
    </main>
    <script src="/endpoint/assets/app.js" defer></script>
  </body>
</html>`;
}
