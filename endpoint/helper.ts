import { execFile } from "child_process";
import path from "path";
import { promisify } from "util";

const execFileAsync = promisify(execFile);

type HelperFailure = {
  ok: false;
  error?: string;
};

function readHelperFailureMessage(payload: unknown, stderr: string, fallbackMessage: string): string {
  if (payload && typeof payload === "object" && "error" in payload && typeof payload.error === "string") {
    return payload.error;
  }

  return stderr.trim() || fallbackMessage;
}

export async function runPythonHelper<Result extends { ok: true }>(
  scriptName: string,
  payload: Record<string, unknown>,
  fallbackMessage: string,
): Promise<Result> {
  const helperPath = path.join(process.cwd(), "endpoint", "scripts", scriptName);

  try {
    const { stdout, stderr } = await execFileAsync("python3", [helperPath, JSON.stringify(payload)]);
    const parsed: unknown = JSON.parse(stdout);

    if (!parsed || typeof parsed !== "object" || !("ok" in parsed) || parsed.ok !== true) {
      throw new Error(readHelperFailureMessage(parsed, stderr, fallbackMessage));
    }

    return parsed as Result;
  } catch (error) {
    const stderr = error instanceof Error && "stderr" in error ? String(error.stderr ?? "") : "";
    const detail = stderr.trim();
    const message = error instanceof Error ? error.message : fallbackMessage;
    throw new Error(detail || message);
  }
}
