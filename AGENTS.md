# AGENTS.md

## Role
- Repo = Express + TypeScript communication-tool prototype.
- Main domains:
  - `hex/`: Hex input parsing APIs + root Hex panel.
  - `endpoint/`: UART/SPI/I2C send APIs + root endpoint panel.
- `endpoint/` uses TypeScript server code plus Python helper scripts.

## Scope
- Ignore `archive/` completely. Do not read, search, or edit it.
- Prefer edits in `server/`, `endpoint/`, `hex/`.
- Never manually edit `dist/`; regenerate with `npm run build`.
- Do not edit `node_modules/`.
- Do not run `3rdparty/setup-debian.sh` unless explicitly requested.
  - Reason: contains `git reset --hard`, `git clean -fd`, `sudo apt install`.

## Map
- `server/`
  - `app.ts`: mounts routes/static assets.
  - `index.ts`: boots server with `HOST` / `PORT`.
  - `ui.ts`: renders `/`, combines endpoint + hex panels, loads shared scripts.
  - `assets/styles.css`: shared layout/panel styles.
  - `assets/hex-input.js`: shared Hex input sanitizer for endpoint + hex UIs.
- `endpoint/`
  - `router.ts`: `/api/endpoint/health`, `/uart/send`, `/spi/send`, `/i2c/send`.
  - `ui.ts`: UART/SPI/I2C config panels.
  - `base.ts`: shared hex/number parse + format helpers.
  - `uart.ts`: UART validation + `/dev/serial0` transfer.
  - `spi.ts`, `i2c.ts`: validation + Python helper execution.
  - `scripts/`: `spi_transfer.py`, `i2c_transfer.py`.
  - `assets/`: endpoint-only frontend assets.
- `hex/`
  - `router.ts`: `/api/hex/parsers`, `/api/hex/parser`.
  - `ui.ts`: Hex parser panel.
  - `registry.ts`, `types.ts`: parser registry/types.
  - `parsers/`: parser implementations (`modbus-rtu`, `midi`, shared base).
  - `assets/app.js`: Hex panel frontend.
  - `scripts/add-parser.js`: parser scaffold generator.
- `3rdparty/`: external environment helpers.
- `README.md`: minimal; trust code over README.

## Commands
- Dev: `npm run dev`
- Build: `npm run build`
- Start built app: `npm run start`
- Add parser: `npm run add:parser -- <type>`

## Invariants
- Keep TypeScript `strict` compatibility.
- Route/static registration pattern is centered in `server/app.ts`; match that shape for new routes/assets.
- `server/ui.ts` depends on shared script load order.
  - Shared frontend utilities must load before panel-specific scripts.
- Hex input policy is shared via `server/assets/hex-input.js`.
  - If input rules change, update shared sanitizer first.
  - Do not diverge `endpoint/assets/app.js` and `hex/assets/app.js`.
- Accepted Hex input: `0-9`, `A-F`, spaces/separators only.
  - `0x` prefix is not allowed.
  - Do not make server parsing disagree with this UI rule.
- Empty Hex input is handled in frontend as `-`.
  - Do not add special empty-input exception paths in parsers/drivers just for this.

## API Contracts
- Hex parser success response:
  - `ok: true`, `type`, `bytes`, `hex`, `length`, `parsed`
- Hex parser failure response:
  - `ok: false`, `type`, `error`
- Endpoint send success response:
  - `ok: true`, `driver`, `config`, `tx`, `rx`
- Endpoint send failure response:
  - `ok: false`, `driver`, `error`
- `hex/assets/app.js` renders `parsed` as raw key/value pairs.
  - Parser outputs should stay flat and human-readable.
- `SPI` / `I2C` Python helpers emit JSON consumed directly by TypeScript.
  - Do not drift helper field names from TS types.

## Change Rules
- Keep driver-specific device validation in each driver file:
  - `endpoint/uart.ts`
  - `endpoint/spi.ts`
  - `endpoint/i2c.ts`
- Keep only shared hex/number handling in `endpoint/base.ts`.
- `endpoint/ui.ts` separates `fields` from `actionField`.
  - Do not repurpose the last regular field to move buttons.
- For new Hex parsers:
  - Preferred: run `npm run add:parser -- <type>` and fill in generated files.
  - Manual path: update `hex/types.ts`, `hex/parsers/`, `hex/registry.ts`, `hex/ui.ts`.
- `hex/scripts/add-parser.js` depends on `PARSER:*` marker comments.
  - Do not remove or corrupt those markers.

## Commit Rules
- Commit message format is fixed:
  - line 1: `TYPE. [SCOPE]`
  - line 2: `- change summary`
- No blank line between title and body.
- Do not append explanation text to the title.
- First body line must start with `- `.
- Use real newlines, not literal `\n`.
- Allowed `TYPE` values:
  - `ADD.`
  - `CHANGE.`
  - `DELETE.`
  - `FORMAT.`
  - `FIXED.`
- `SCOPE` must be uppercase tags such as `[HEX]`, `[COMMON]`, `[README]`, `[ARCH]`.
- If multiple domains are mixed, prefer `[COMMON]`.

## Pre-Commit Review
- Before any commit, complete all 3 checks:
  1. `git diff --cached --name-only`
  2. `git diff --cached`
  3. Verify commit title/body match both rules and actual staged changes.

## Verify
- Required: `npm run build`
- If Python helpers changed, also run:
  - `python3 -m py_compile endpoint/scripts/i2c_transfer.py endpoint/scripts/spi_transfer.py`
