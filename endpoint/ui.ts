type DriverType = "uart" | "spi" | "i2c";

type OptionSpec = {
  value: string;
  label: string;
};

type BaseFieldSpec = {
  key: string;
  label: string;
};

type InputFieldSpec = BaseFieldSpec & {
  control: "input";
  inputType: "number" | "text";
  value: string;
};

type SelectFieldSpec = BaseFieldSpec & {
  control: "select";
  options: OptionSpec[];
};

type ActionFieldSpec = BaseFieldSpec & {
  control: "actions";
  inputType: "number" | "text";
  value: string;
};

type FieldSpec = InputFieldSpec | SelectFieldSpec | ActionFieldSpec;

type DriverSection = {
  type: DriverType;
  fields: FieldSpec[];
};

const driverSections: DriverSection[] = [
  {
    type: "uart",
    fields: [
      { key: "device-path", label: "DEVICE", control: "input", inputType: "text", value: "/dev/serial0" },
      { key: "baud-rate", label: "BAUD RATE", control: "input", inputType: "number", value: "115200" },
      {
        key: "data-bits",
        label: "DATA BITS",
        control: "select",
        options: [
          { value: "8", label: "8" },
          { value: "7", label: "7" },
        ],
      },
      {
        key: "parity",
        label: "PARITY",
        control: "select",
        options: [
          { value: "none", label: "none" },
          { value: "even", label: "even" },
          { value: "odd", label: "odd" },
        ],
      },
      {
        key: "stop-bits",
        label: "STOP BITS",
        control: "select",
        options: [
          { value: "1", label: "1" },
          { value: "2", label: "2" },
        ],
      },
      { key: "timeout", label: "TIMEOUT", control: "actions", inputType: "number", value: "1000" },
    ],
  },
  {
    type: "spi",
    fields: [
      { key: "device-path", label: "DEVICE", control: "input", inputType: "text", value: "/dev/spidev0.0" },
      { key: "speed", label: "SPEED", control: "input", inputType: "number", value: "1000000" },
      {
        key: "mode",
        label: "MODE",
        control: "select",
        options: [
          { value: "0", label: "mode 0" },
          { value: "1", label: "mode 1" },
          { value: "2", label: "mode 2" },
          { value: "3", label: "mode 3" },
        ],
      },
      { key: "bits-per-word", label: "BITS", control: "input", inputType: "number", value: "8" },
      { key: "delay", label: "DELAY", control: "actions", inputType: "number", value: "0" },
    ],
  },
  {
    type: "i2c",
    fields: [
      { key: "device-path", label: "DEVICE", control: "input", inputType: "text", value: "/dev/i2c-1" },
      { key: "address", label: "ADDRESS", control: "input", inputType: "text", value: "0x3C" },
      { key: "speed", label: "SPEED", control: "input", inputType: "number", value: "100000" },
      { key: "read-length", label: "READ LENGTH", control: "input", inputType: "number", value: "0" },
      { key: "timeout", label: "TIMEOUT", control: "actions", inputType: "number", value: "1000" },
    ],
  },
];

function renderOptions(options: OptionSpec[]): string {
  return options
    .map((option, index) => {
      const selected = index === 0 ? " selected" : "";
      return `<option value="${option.value}"${selected}>${option.label}</option>`;
    })
    .join("");
}

function renderFieldControl(driverType: DriverType, field: FieldSpec): string {
  const id = `endpoint-${driverType}-${field.key}`;

  if (field.control === "select") {
    return `<select id="${id}">${renderOptions(field.options)}</select>`;
  }

  if (field.control === "actions") {
    const clearAttrs =
      driverType === "uart"
        ? 'id="endpoint-clear-button"'
        : 'data-role="endpoint-clear-mirror"';
    const sendAttrs =
      driverType === "uart"
        ? 'id="endpoint-send-button"'
        : 'data-role="endpoint-send-mirror"';

    return `<div class="setting-value-row">
            <input id="${id}" type="${field.inputType}" value="${field.value}" />
            <div class="action-row">
              <button ${clearAttrs} type="button" class="action-button secondary">clear</button>
              <button ${sendAttrs} type="button" class="action-button">send</button>
            </div>
          </div>`;
  }

  return `<input id="${id}" type="${field.inputType}" value="${field.value}" />`;
}

function renderField(driverType: DriverType, field: FieldSpec, visible: boolean): string {
  const actionClass = field.control === "actions" ? " setting-field-actions" : "";
  const hiddenAttr = visible ? "" : " hidden";

  return `<label class="setting-field${actionClass}" data-driver="${driverType}"${hiddenAttr}>
          <span class="setting-label">${field.label}</span>
          ${renderFieldControl(driverType, field)}
        </label>`;
}

function renderDriverSection(section: DriverSection, visible: boolean): string {
  return section.fields.map((field) => renderField(section.type, field, visible)).join("\n");
}

function renderDriverOptions(): string {
  return driverSections
    .map((section, index) => {
      const selected = index === 0 ? " selected" : "";
      return `<option value="${section.type}"${selected}>${section.type}</option>`;
    })
    .join("");
}

function renderDriverFields(): string {
  return driverSections
    .map((section, index) => renderDriverSection(section, index === 0))
    .join("\n");
}

export function renderEndpointPanel(): string {
  return `<section class="page-panel endpoint-panel">
    <div class="input-box">
      <div class="control-row">
        <div class="select-wrap">
          <select id="endpoint-driver-type" class="top-select">
            ${renderDriverOptions()}
          </select>
        </div>
      </div>
      <div class="settings-grid settings-inline">
        ${renderDriverFields()}
      </div>
      <pre id="endpoint-result-output" class="parsed-output">-</pre>
    </div>
    <textarea
      id="endpoint-hex-input"
      class="raw-input"
      spellcheck="false"
      placeholder="INPUT HEX"
    ></textarea>
  </section>`;
}
