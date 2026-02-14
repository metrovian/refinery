#pragma once
#include <Windows.h>
#include <mmsystem.h>
#include <cmath>
#pragma comment(lib, "winmm.lib")

constexpr BYTE MIDI_KEY_DOWN = 154;
constexpr BYTE MIDI_KEY_REL = 138;
constexpr BYTE MIDI_SUSTAIN = 186;
constexpr BYTE MIDI_PITCH = 234;

constexpr double SONIC = 343000.000;

constexpr double PI = 3.1415926;
constexpr double Z0 = 0.0;

constexpr double A0 = 27.5000;
constexpr double B0 = 30.8677;
constexpr double C1 = 32.7032;
constexpr double D1 = 36.7081;
constexpr double E1 = 41.2034;
constexpr double F1 = 43.6535;
constexpr double G1 = 48.9994;

constexpr double A1 = 55.0000;
constexpr double B1 = 61.7354;
constexpr double C2 = 65.4064;
constexpr double D2 = 73.4162;
constexpr double E2 = 82.4069;
constexpr double F2 = 87.3071;
constexpr double G2 = 97.9989;

constexpr double A2 = 110.0000;
constexpr double B2 = 123.4708;
constexpr double C3 = 130.8128;
constexpr double D3 = 146.8324;
constexpr double E3 = 164.8138;
constexpr double F3 = 174.6141;
constexpr double G3 = 195.9977;

constexpr double A3 = 220.0000;
constexpr double B3 = 246.9417;
constexpr double C4 = 261.6256;
constexpr double D4 = 293.6648;
constexpr double E4 = 329.6276;
constexpr double F4 = 349.2282;
constexpr double G4 = 391.9954;

constexpr double A4 = 440.0000;
constexpr double B4 = 493.8833;
constexpr double C5 = 523.2511;
constexpr double D5 = 587.3295;
constexpr double E5 = 659.2551;
constexpr double F5 = 698.4565;
constexpr double G5 = 783.9909;

constexpr double A5 = 880.0000;
constexpr double B5 = 987.7666;
constexpr double C6 = 1046.5023;
constexpr double D6 = 1174.6591;
constexpr double E6 = 1318.5102;
constexpr double F6 = 1396.9129;
constexpr double G6 = 1567.9817;