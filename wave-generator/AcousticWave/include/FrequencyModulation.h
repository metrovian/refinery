#pragma once
#include "WaveFunction.h"

class FrequencyModulation
{
public: /* public use */
	static WaveFunction vibrato(WaveFunction _wave, double _freq, double _vfreq, double _vdepth);
	static WaveFunction bending(WaveFunction _wave, double _freq, double _bfreq, double _btime);
};