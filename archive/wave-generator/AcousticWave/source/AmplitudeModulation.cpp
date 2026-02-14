#include "AmplitudeModulation.h"

WaveFunction AmplitudeModulation::envelopeADS(WaveFunction _wave, double _attack, double _decay, double _sustain)
{
	WaveFunction ret;
	WaveHeader head = _wave.getWaveHeader();
	WaveData dat = _wave.getWaveData();

	std::vector<short> res(dat.size());

	double tau = -1;

	for (size_t i = 0; i < res.size(); ++i)
	{
		if (i < (size_t)(_attack * (double)head.SAMPLE_RATE))
		{
			tau = _attack / std::log(1.0 - 1.0 / exp(1.0));
			res[i] = std::exp(1.0) * dat[i] * (1.0 - std::exp((double)i / (double)head.SAMPLE_RATE / tau));
		}

		else if (i < (size_t)((_attack + _decay) * (double)head.SAMPLE_RATE))
		{
			tau = _decay / std::log(_sustain);
			res[i] = dat[i] * std::exp(((double)i / (double)head.SAMPLE_RATE - _attack) / tau);
		}

		else
		{
			res[i] = _sustain * dat[i];
		}
	}

	ret.setWaveFunction(res, head);

	return ret;
}
