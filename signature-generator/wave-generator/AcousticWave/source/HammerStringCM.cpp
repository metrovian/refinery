#include "HammerStringCM.h"

HammerStringCM::HammerStringCM(double _namp, double _freq, double _dura, unsigned int _srate, unsigned short _sbit)
{
	synthesis(_namp, _freq, _dura, _srate, _sbit);
}

HammerStringCM::HammerStringCM(double _namp, double _freq, double _dura, unsigned int _srate, unsigned short _sbit, double _decay, double _mod[3], double _imps[3], double _impe[3])
{
	for (unsigned char i = 0; i < 3; ++i)
	{
		imps[i] = _imps[i];
		impe[i] = _impe[i];
		modulus[i] = _mod[i];
	}

	synthesis(_namp, _freq, _dura, _srate, _sbit);
}

bool HammerStringCM::synthesis(double _namp, double _freq, double _dura, unsigned int _srate, unsigned short _sbit)
{
	setWaveHeader(_srate, _sbit);

	DelayData impulse[3];
	DelayData cmsum;

	for (unsigned char i = 0; i < 3; ++i)
	{
		impulse[i] = calcImpulseDelayLine(_namp, _freq, imps[i], impe[i]);
		impulse[i] = passSoundBoardCMF(impulse[i], _freq, invp, _srate, _sbit);
		impulse[i] = passStringElasticModulusLPF(impulse[i], modulus[i]);
	}

	size_t size = impulse[0].size();
	for (size_t i = 0; i < size; ++i)
	{
		cmsum.push((short)((impulse[0].front() + impulse[1].front() + impulse[2].front()) / 3.0));

		impulse[0].pop();
		impulse[1].pop();
		impulse[2].pop();
	}

	WaveData dat(calcWaveDataSize(_dura, _srate));

	if (cmsum.size() > 0)
	{
		dat[0] = passDynamicLPF(cmsum, 0, _freq);

		cmsum.push(passStringDF(cmsum, _freq, 0.1, decay));
		cmsum.pop();

		for (size_t i = 1; i < dat.size(); ++i)
		{
			dat[i] = passDynamicLPF(cmsum, dat[i - 1], _freq);

			cmsum.push(passStringDF(cmsum, _freq, 0.1, decay));
			cmsum.pop();
		}
	}

	setWaveData(dat);
	return true;
}