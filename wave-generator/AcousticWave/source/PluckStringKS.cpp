#include "PluckStringKS.h"

PluckStringKS::PluckStringKS(double _namp, double _freq, double _dura, unsigned int _srate, unsigned short _sbit)
{
	synthesis(_namp, _freq, _dura, _srate, _sbit);
}

PluckStringKS::PluckStringKS(double _namp, double _freq, double _dura, unsigned int _srate, unsigned short _sbit, unsigned char _pow)
{
	pow = _pow;
	synthesis(_namp, _freq, _dura, _srate, _sbit);
}

bool PluckStringKS::synthesis(double _namp, double _freq, double _dura, unsigned int _srate, unsigned short _sbit)
{
	setWaveHeader(_srate, _sbit);

	DelayData rand = calcRandomDelayLine(_namp, _freq);
	WaveData dat(calcWaveDataSize(_dura, _srate));

	if (rand.size() > 0)
	{
		for (size_t i = 0; i < dat.size(); ++i)
		{
			dat[i] = rand.front();

			rand.push(passSimpleLPF(rand, pow));
			rand.pop();
		}
	}

	setWaveData(dat);
	return true;
}

PluckStringEKS::PluckStringEKS(double _namp, double _freq, double _dura, unsigned int _srate, unsigned short _sbit)
{
	synthesis(_namp, _freq, _dura, _srate, _sbit);
}

PluckStringEKS::PluckStringEKS(double _namp, double _freq, double _dura, unsigned int _srate, unsigned short _sbit, double _decay, double _damp, double _tune, double _mod, double _pos)
{
	decay = _decay;
	damp = _damp;
	tune = _tune;
	modulus = _mod;
	pos = _pos;

	synthesis(_namp, _freq, _dura, _srate, _sbit);
}

bool PluckStringEKS::synthesis(double _namp, double _freq, double _dura, unsigned int _srate, unsigned short _sbit)
{
	setWaveHeader(_srate, _sbit);

	WaveData dat(calcWaveDataSize(_dura, _srate));
	DelayData rand = calcRandomDelayLine(_namp, _freq);

	if (rand.size() > 0)
	{
		DelayData proc1 = passStringElasticModulusLPF(rand, modulus);
		DelayData proc2 = passStringPositionCF(proc1, pos);
		DelayData proc3;

		DelayData proc2m1 = proc2; proc2m1.pop();

		double buf = 0;
		auto flap = [&]()
			{
				proc3.push(passStringDF(proc2m1, _freq, damp, decay));

				proc2.push(passStringAPF(proc3, buf, tune));
				proc2m1.push(passStringAPF(proc3, buf, tune));
				proc2m1.pop();

				buf = proc2.back();
				proc2.pop();
				proc3.pop();
			};

		dat[0] = passDynamicLPF(proc2, 0, _freq);
		proc3.push(passStringDF(proc2, _freq, damp, decay));

		flap();

		for (size_t i = 1; i < dat.size(); ++i)
		{
			dat[i] = passDynamicLPF(proc2, dat[i - 1], _freq);
			flap();
		}
	}

	setWaveData(dat);
	return true;
}
