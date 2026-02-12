#pragma once
#include "WaveFunction.h"

#include <Windows.h>
#include <mmsystem.h>
#include <iostream>
#include <thread>

class MIDI
{
protected: /* data */
	WaveFunction sound[88];

private: /* config */
	double pmax = 1.0;
	std::vector<double> vamps = vampConstant();

private: /* control */
	bool sustain = false;
	double pchs = 1.0;

private: /* handle */
	HMIDIIN midi;
	static void CALLBACK inputCallback(HMIDIIN hMidiIn, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);

private: /* parts */
	void setPchValue(char _pch);

public: /* public use */
	bool open(unsigned int _id);
	bool start();
	bool stop();
	bool close();

	bool setSound(WaveFunction(*_wave)(double _freq, double _dura), double _fdura);

	bool setPchMaximum(double _pmax);
	bool setVampFunction(std::vector<double> _vamps);

	static std::vector<double> vampConstant();
	static std::vector<double> vampLinear();
};