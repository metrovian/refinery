#include "BowStringLP.h"

double BowStringLP::linearBow(double _decay, double _bwmax, double _bwrel, double _rtime) const
{
	double min = 0.01;

	if (_rtime < _bwmax)
	{
		return min + _rtime * decay / bwmax;
	}

	else if (_rtime < _bwrel)
	{
		return min + decay;
	}

	else
	{
		return min + (1.0 - _rtime) * decay;
	}
}

BowStringLP::BowStringLP(double _namp, double _freq, double _dura, unsigned int _srate, unsigned short _sbit)
{
	synthesis(_namp, _freq, _dura, _srate, _sbit);
}

BowStringLP::BowStringLP(double _namp, double _freq, double _dura, unsigned int _srate, unsigned short _sbit, unsigned char _band, unsigned char _pnum, double _mod, double _pos, double _decay, double _bwmax, double _bwrel)
{
	band = _band;
	pnum = _pnum;
	modulus = _mod;
	pos = _pos;
	decay = _decay;
	bwmax = _bwmax;
	bwrel = _bwrel;

	synthesis(_namp, _freq, _dura, _srate, _sbit);
}

bool BowStringLP::synthesis(double _namp, double _freq, double _dura, unsigned int _srate, unsigned short _sbit)
{
	setWaveHeader(_srate, _sbit);

	WaveData dat(calcWaveDataSize(_dura, _srate));
	DelayData train = calcBandLimDelayLine(_namp, _freq, band);

	if (train.size() > 0)
	{
		DelayData raw1 = passStringElasticModulusLPF(train, modulus);
		DelayData raw2 = passStringPositionCF(raw1, pos);
		DelayData proc1 = raw2;
		DelayData proc2 = proc1;

		dat[0] = 0;

		for (size_t i = 1; i < dat.size(); ++i)
		{
			dat[i] = passDynamicLPF(proc2, dat[i - 1], _freq);

			raw2.push(raw2.front());
			proc1.push(passStringDF(raw2, _freq, 0.0, linearBow(decay, bwmax, bwrel, (double)i / (double)dat.size())));
			proc2.push(passStringDF(proc1, _freq, 0.5, decay));

			raw2.pop();
			proc1.pop();
			proc2.pop();
		}

		setWaveData(passAutoRegressionLPC(dat, pnum));
	}

	else
	{
		setWaveData(dat);
	}

	return true;
}
