import { parseBytes, toHexByte } from "./base";
import { ParseResult } from "../types";

type ParsedMidiMessage = Record<string, unknown>;

function expectLength(bytes: number[], expected: number, messageName: string): void {
  if (bytes.length !== expected) {
    throw new Error(`${messageName} message must be ${expected} byte(s), received ${bytes.length}.`);
  }
}

function noteName(noteNumber: number): string {
  const names = ["C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"];
  const octave = Math.floor(noteNumber / 12);
  return `${names[noteNumber % 12]}${octave}`;
}

function controlChangeName(controller: number): string {
  const names: Record<number, string> = {
    0x00: "bank-select",
    0x01: "modulation-wheel",
    0x02: "breath-controller",
    0x04: "foot-controller",
    0x05: "portamento-time",
    0x06: "data-entry-msb",
    0x07: "channel-volume",
    0x08: "balance",
    0x0a: "pan",
    0x0b: "expression-controller",
    0x40: "sustain-pedal",
    0x41: "portamento",
    0x42: "sostenuto",
    0x43: "soft-pedal",
    0x46: "sound-controller-1",
    0x47: "sound-controller-2",
    0x48: "sound-controller-3",
    0x49: "sound-controller-4",
    0x4a: "sound-controller-5",
    0x4b: "sound-controller-6",
    0x4c: "sound-controller-7",
    0x4d: "sound-controller-8",
    0x4e: "sound-controller-9",
    0x4f: "sound-controller-10",
    0x5b: "effects-depth-1",
    0x5c: "effects-depth-2",
    0x5d: "effects-depth-3",
    0x5e: "effects-depth-4",
    0x5f: "effects-depth-5",
    0x78: "all-sound-off",
    0x79: "reset-all-controllers",
    0x7a: "local-control",
    0x7b: "all-notes-off",
    0x7c: "omni-mode-off",
    0x7d: "omni-mode-on",
    0x7e: "mono-mode-on",
    0x7f: "poly-mode-on",
  };

  return names[controller] ?? "unknown-controller";
}

function decodeChannelVoice(bytes: number[]): ParsedMidiMessage {
  const status = bytes[0];
  const messageType = status & 0xf0;
  const channel = (status & 0x0f) + 1;

  switch (messageType) {
    case 0x80: {
      expectLength(bytes, 3, "Note Off");
      const note = bytes[1];
      const velocity = bytes[2];
      return {
        category: "channel-voice",
        message: "note-off",
        channel,
        noteNumber: note,
        noteName: noteName(note),
        velocity,
      };
    }
    case 0x90: {
      expectLength(bytes, 3, "Note On");
      const note = bytes[1];
      const velocity = bytes[2];
      return {
        category: "channel-voice",
        message: velocity === 0 ? "note-on (velocity-zero)" : "note-on",
        channel,
        noteNumber: note,
        noteName: noteName(note),
        velocity,
        interpretedAs: velocity === 0 ? "note-off" : "note-on",
      };
    }
    case 0xa0: {
      expectLength(bytes, 3, "Polyphonic Key Pressure");
      const note = bytes[1];
      return {
        category: "channel-voice",
        message: "polyphonic-key-pressure",
        channel,
        noteNumber: note,
        noteName: noteName(note),
        pressure: bytes[2],
      };
    }
    case 0xb0: {
      expectLength(bytes, 3, "Control Change");
      const controller = bytes[1];
      const value = bytes[2];
      return {
        category: "channel-voice",
        message: "control-change",
        channel,
        controller,
        controllerName: controlChangeName(controller),
        value,
      };
    }
    case 0xc0:
      expectLength(bytes, 2, "Program Change");
      return {
        category: "channel-voice",
        message: "program-change",
        channel,
        programNumber: bytes[1],
      };
    case 0xd0:
      expectLength(bytes, 2, "Channel Pressure");
      return {
        category: "channel-voice",
        message: "channel-pressure",
        channel,
        pressure: bytes[1],
      };
    case 0xe0: {
      expectLength(bytes, 3, "Pitch Bend");
      const rawValue = (bytes[2] << 7) | bytes[1];
      return {
        category: "channel-voice",
        message: "pitch-bend",
        channel,
        lsb: bytes[1],
        msb: bytes[2],
        rawValue,
        signedValue: rawValue - 8192,
      };
    }
    default:
      throw new Error(`Unsupported MIDI channel message: ${toHexByte(status)}`);
  }
}

function decodeManufacturerId(data: number[]): { manufacturerId: string; manufacturerBytes: string[]; payloadOffset: number } {
  if (data.length === 0) {
    throw new Error("System Exclusive message must include a manufacturer ID.");
  }

  if (data[0] === 0x00) {
    if (data.length < 3) {
      throw new Error("Extended System Exclusive manufacturer ID requires 3 bytes.");
    }
    return {
      manufacturerId: `0x${data.slice(0, 3).map(toHexByte).join("")}`,
      manufacturerBytes: data.slice(0, 3).map(toHexByte),
      payloadOffset: 3,
    };
  }

  return {
    manufacturerId: `0x${toHexByte(data[0])}`,
    manufacturerBytes: [toHexByte(data[0])],
    payloadOffset: 1,
  };
}

function decodeSystemMessage(bytes: number[]): ParsedMidiMessage {
  const status = bytes[0];

  switch (status) {
    case 0xf0: {
      if (bytes.length < 2) {
        throw new Error("System Exclusive message must include terminating F7.");
      }
      if (bytes[bytes.length - 1] !== 0xf7) {
        throw new Error("System Exclusive message must terminate with F7.");
      }

      const body = bytes.slice(1, -1);
      const manufacturer = decodeManufacturerId(body);
      return {
        category: "system-common",
        message: "system-exclusive",
        manufacturerId: manufacturer.manufacturerId,
        manufacturerBytes: manufacturer.manufacturerBytes,
        data: body.slice(manufacturer.payloadOffset).map(toHexByte),
        terminated: true,
      };
    }
    case 0xf1:
      expectLength(bytes, 2, "MIDI Time Code Quarter Frame");
      return {
        category: "system-common",
        message: "midi-time-code-quarter-frame",
        messageType: bytes[1] >> 4,
        value: bytes[1] & 0x0f,
      };
    case 0xf2: {
      expectLength(bytes, 3, "Song Position Pointer");
      const position = (bytes[2] << 7) | bytes[1];
      return {
        category: "system-common",
        message: "song-position-pointer",
        lsb: bytes[1],
        msb: bytes[2],
        position,
      };
    }
    case 0xf3:
      expectLength(bytes, 2, "Song Select");
      return {
        category: "system-common",
        message: "song-select",
        songNumber: bytes[1],
      };
    case 0xf6:
      expectLength(bytes, 1, "Tune Request");
      return {
        category: "system-common",
        message: "tune-request",
      };
    case 0xf7:
      return {
        category: "system-common",
        message: "end-of-exclusive",
        data: bytes.slice(1).map(toHexByte),
      };
    case 0xf8:
      expectLength(bytes, 1, "Timing Clock");
      return {
        category: "system-realtime",
        message: "timing-clock",
      };
    case 0xfa:
      expectLength(bytes, 1, "Start");
      return {
        category: "system-realtime",
        message: "start",
      };
    case 0xfb:
      expectLength(bytes, 1, "Continue");
      return {
        category: "system-realtime",
        message: "continue",
      };
    case 0xfc:
      expectLength(bytes, 1, "Stop");
      return {
        category: "system-realtime",
        message: "stop",
      };
    case 0xfe:
      expectLength(bytes, 1, "Active Sensing");
      return {
        category: "system-realtime",
        message: "active-sensing",
      };
    case 0xff:
      expectLength(bytes, 1, "System Reset");
      return {
        category: "system-realtime",
        message: "system-reset",
      };
    default:
      throw new Error(`Unsupported MIDI system message: ${toHexByte(status)}`);
  }
}

function decodeMidiMessage(bytes: number[]): ParsedMidiMessage {
  if (bytes.length === 0) {
    return {
      parser: "midi",
      message: "empty",
      category: "none",
    };
  }

  const status = bytes[0];
  if (status < 0x80) {
    throw new Error("MIDI message must start with a status byte (>= 0x80). Running status is not supported.");
  }

  const parsed = status < 0xf0 ? decodeChannelVoice(bytes) : decodeSystemMessage(bytes);
  return {
    parser: "midi",
    statusByte: toHexByte(status),
    ...parsed,
  };
}

export function parseMidi(input: string): ParseResult {
  try {
    const bytes = parseBytes(input);
    return {
      ok: true,
      type: "midi",
      bytes,
      hex: bytes.map(toHexByte),
      length: bytes.length,
      parsed: decodeMidiMessage(bytes),
    };
  } catch (error) {
    const message = error instanceof Error ? error.message : "Unknown parse error";
    return { ok: false, type: "midi", error: message };
  }
}
