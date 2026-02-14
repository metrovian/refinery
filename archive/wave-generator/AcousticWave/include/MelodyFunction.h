#pragma once
#include "WaveFunction.h"
#include "FourierTransform.h"

class MelodyFunction
{
private: /* data */
	std::vector<double> dynamic;
	std::vector<double> pitch;
	std::vector<double> beat;

private: /* parts */
	bool isPlayable();

public: /* public use */
	WaveFunction getWaveFunction(WaveFunction (*_wave)(double _namp, double _freq, double _dura));
	WaveFunction getWaveFunction(FourierFunction (*_wave)(double _namp, double _freq, double _dura));

public: /* examples */
	static MelodyFunction over_the_rainbow(double _bpm);
	static MelodyFunction cannon(double _bpm);
};