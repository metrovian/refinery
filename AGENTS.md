# AGENTS.md

## 목적
- 이 저장소는 Express + TypeScript 기반의 통신 도구 프로토타입입니다.
- 현재 핵심 기능은 Hex 입력 기반 `MODBUS-RTU`/`MIDI` 파싱 API와 루트 UI(`/`)의 Hex 패널입니다.
- 루트 UI에는 `endpoint` 패널도 함께 포함되지만, 실제 전송 로직은 아직 연결되지 않은 스캐폴드 상태입니다.

## 작업 범위 규칙
- `archive/` 디렉터리는 무시합니다. (읽기/수정/탐색 대상에서 제외)
- 애플리케이션 소스 수정은 `server/`, `endpoint/`, `hex/`를 우선합니다.
- `dist/`는 빌드 산출물입니다. 필요 시 `npm run build`로 재생성하며 수동 편집하지 않습니다.
- `node_modules/`는 수정하지 않습니다.
- 외부 환경 보조 스크립트는 `3rdparty/`에 있으며, 특히 `setup-debian.sh`는 `git reset --hard`, `git clean -fd`, `sudo apt install`을 포함하므로 명시적 요청 없이 실행하지 않습니다.

## 디렉터리 구조 (archive 제외)
- `server/`: 앱 초기화 및 서버 부트스트랩
  - `app.ts`: 미들웨어/라우터 등록
  - `index.ts`: `HOST`/`PORT`로 서버 실행
  - `ui.ts`: 루트 HTML 렌더링, `endpoint`/`hex` 패널 조합
  - `assets/styles.css`: 공통 레이아웃/패널 스타일
- `endpoint/`: 일반 엔드포인트 라우터
  - `router.ts`: `/api/endpoint/health`
  - `ui.ts`: UART/SPI/I2C 설정 패널 렌더링
  - `assets/`: 엔드포인트 패널 전용 스크립트/스타일
- `hex/`: Hex 파싱 도메인
  - `router.ts`: `/api/hex/parsers`, `/api/hex/parser`
  - `ui.ts`: Hex 파서 패널 렌더링
  - `registry.ts`, `types.ts`: 파서 타입/레지스트리
  - `parsers/`: 파서 구현 (`base.ts`, `modbus-rtu.ts`, `midi.ts`)
  - `assets/`: Hex 패널 프런트엔드 스크립트 (`app.js`)
  - `scripts/add-parser.js`: 새 파서 스캐폴딩 스크립트
- `3rdparty/`: 외부 환경 보조 스크립트
- `README.md`: 매우 간단한 개요만 있으므로, 상세 구조 판단은 실제 코드 기준으로 합니다.

## 실행/빌드 명령
- 개발 서버: `npm run dev`
- 빌드: `npm run build`
- 실행(빌드 결과): `npm run start`
- 파서 스캐폴딩: `npm run add:parser -- <type>`

## 코드 변경 가이드
- TypeScript `strict` 모드 기준으로 타입 안정성을 유지합니다.
- 서버는 `server/app.ts`에서 `/api/endpoint`, `/api/hex`, 정적 자산 경로를 묶어서 등록합니다. 새 라우트/자산을 추가할 때 이 조합을 기준으로 맞춥니다.
- Hex 파서 API 응답 스키마는 아래 형태를 유지합니다.
  - 성공: `ok: true`, `type`, `bytes`, `hex`, `length`, `parsed`
  - 실패: `ok: false`, `type`, `error`
- `hex/assets/app.js`는 `parsed` 객체의 key/value를 그대로 표시하므로, 파서 구현은 사람이 읽을 수 있는 평탄한 필드를 우선 반환합니다.
- 새 Hex 파서를 추가할 때는 두 방법 중 하나를 사용합니다.
  1. 권장: `npm run add:parser -- <type>` 실행 후 생성된 파일을 구체화합니다.
  2. 수동: `hex/types.ts`, `hex/parsers/`, `hex/registry.ts`, `hex/ui.ts`를 함께 수정합니다.
- `hex/scripts/add-parser.js`는 마커 주석(`PARSER:*`)에 의존하므로, 관련 파일에서 해당 마커를 제거하거나 훼손하지 않습니다.
- `endpoint/` 프런트엔드는 현재 UI 동작만 있고 백엔드 전송 API는 없습니다. `send` 동작을 실제화할 때는 라우터, UI, 자산을 함께 설계해야 합니다.

## 커밋 메시지 규칙 (확정)
- 사용자 스타일 고정 템플릿:
  - 1행(제목): `TYPE. [SCOPE]`
  - 2행(내용): `- 변경 내용`
- 제목에 설명을 붙이지 않습니다. (`- ...`를 제목에 쓰지 않음)
- 제목 다음 줄부터 바로 본문을 작성합니다. (빈 줄 금지)
- 본문 첫 줄은 반드시 `- `로 시작합니다.
- 메시지에 `\n` 문자열을 입력하지 않고 실제 줄바꿈만 사용합니다.

### TYPE/SCOPE 규칙
- `TYPE` 허용값:
  - `ADD.`
  - `CHANGE.`
  - `DELETE.`
  - `FORMAT.`
  - `FIXED.`
- `SCOPE`는 대문자 태그 사용:
  - 예: `[HEX]`, `[COMMON]`, `[README]`, `[ARCH]`
- 여러 도메인이 섞이면 `SCOPE`는 `[COMMON]`을 우선합니다.

### 커밋 전 리뷰 강제 절차
- 아래 점검 3개를 모두 완료하기 전에는 커밋하지 않습니다.
  1. `git diff --cached --name-only`로 스테이징 파일 목록 검토
  2. `git diff --cached`로 변경 내용 검토
  3. 커밋 제목/본문이 규칙과 실제 변경사항에 일치하는지 검토

### 작성 예시
- `ADD. [COMMON]`
- `- AGENTS.md 파일 업로드`

## 검증 기준
- 필수 검증: `npm run build` 성공
