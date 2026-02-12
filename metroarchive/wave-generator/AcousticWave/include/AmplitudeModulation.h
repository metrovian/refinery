#pragma once
#include "WaveFunction.h"

class AmplitudeModulation
{
public: /* public use */
	static WaveFunction envelopeADS(WaveFunction _wave, double _attack, double _decay, double _sustain);
};