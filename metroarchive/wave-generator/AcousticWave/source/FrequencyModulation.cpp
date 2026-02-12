#include "FrequencyModulation.h"
#include "Predefined.h"

WaveFunction FrequencyModulation::vibrato(WaveFunction _wave, double _freq, double _vfreq, double _vdepth)
{
	WaveFunction ret;
	WaveHeader head = _wave.getWaveHeader();
	WaveData dat = _wave.getWaveData();

	std::vector<double> vibration(dat.size());
	std::vector<short> res(dat.size());

	double rate = 2.0 * PI * _vfreq;

	for (size_t i = 0; i < vibration.size(); ++i)
	{
		vibration[i] = _vdepth / _freq / rate * cos(rate * (double)i / (double)head.SAMPLE_RATE);
		res[i] = dat[(size_t)((double)i + (double)head.SAMPLE_RATE * (vibration[0] - vibration[i])) % dat.size()];
	}

	ret.setWaveFunction(res, head);

	return ret;
}

WaveFunction FrequencyModulation::bending(WaveFunction _wave, double _freq, double _bfreq, double _btime)
{
	WaveFunction ret;
	WaveHeader head = _wave.getWaveHeader();
	WaveData dat = _wave.getWaveData();

	std::vector<double> vibration(dat.size());
	std::vector<short> res(dat.size());

	for (size_t i = 0; i < vibration.size(); ++i)
	{
		if ((double)i / (double)head.SAMPLE_RATE > _btime)
		{
			vibration[i] = (_bfreq - _freq) / _freq * (double)i;
		}

		else
		{
			vibration[i] = (_bfreq - _freq) / _freq / _btime * (double)i * (double)i / 2.0 / (double)head.SAMPLE_RATE;
		}

		res[i] = dat[(size_t)((double)i + vibration[i]) % dat.size()];
	}

	ret.setWaveFunction(res, head);

	return ret;
}
