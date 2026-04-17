# AGENTS.md

## 목적
- 이 저장소는 Express + TypeScript 기반의 간단한 파서 서비스입니다.
- 현재 핵심 기능은 Hex 입력 기반 `CAN` 파싱 API와 `/hex` UI 페이지입니다.

## 작업 범위 규칙
- `archive/` 디렉터리는 무시합니다. (읽기/수정/탐색 대상에서 제외)
- 애플리케이션 소스 수정은 `server/`, `endpoint/`, `hex/`를 우선합니다.
- `dist/`는 빌드 산출물입니다. 필요 시 `npm run build`로 재생성하며 수동 편집하지 않습니다.
- `node_modules/`는 수정하지 않습니다.

## 디렉터리 구조 (archive 제외)
- `server/`: 앱 초기화 및 서버 부트스트랩
  - `app.ts`: 미들웨어/라우터 등록
  - `index.ts`: `HOST`/`PORT`로 서버 실행
- `endpoint/`: 일반 엔드포인트 라우터
  - `router.ts`: `/api/endpoint/health`
- `hex/`: Hex 파싱 도메인
  - `api-router.ts`: `/api/hex/parsers`, `/api/hex/parse`
  - `page-router.ts`, `ui.ts`: `/hex` 페이지 렌더링
  - `registry.ts`, `types.ts`: 파서 타입/레지스트리
  - `parsers/`: 파서 구현 (`base.ts`, `can.ts`)
  - `assets/`: 프런트엔드 정적 파일 (`app.js`, `styles.css`)
- `3rdparty/`: 외부 환경 보조 스크립트

## 실행/빌드 명령
- 개발 서버: `npm run dev`
- 빌드: `npm run build`
- 실행(빌드 결과): `npm run start`

## 코드 변경 가이드
- TypeScript `strict` 모드 기준으로 타입 안정성을 유지합니다.
- 새 Hex 파서를 추가할 때:
  1. `hex/types.ts`의 `ParseType` 확장
  2. `hex/parsers/`에 구현 추가
  3. `hex/registry.ts`에 등록
  4. 필요 시 `hex/ui.ts`, `hex/assets/app.js`의 파서 선택 UI 반영
- API 응답 스키마는 `ok`/`error` 구조를 유지해 기존 UI 동작과 호환되게 변경합니다.

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
- 최소 검증: `npm run build` 성공
- 동작 확인:
  - `GET /api/endpoint/health`
  - `GET /api/hex/parsers`
  - `POST /api/hex/parse` with `{ "type": "can", "input": "18FEF100 01 0A FF 1C" }`
  - `GET /hex` UI 정상 렌더링
