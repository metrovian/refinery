#include "FDTD.h"
#include "Predefined.h"

bool FDTD::setBasicCondition(double _wspeed, double _length, double _period, size_t _numt)
{
	if (_length < 0) return false;
	if (_period < 0) return false;
	if (_wspeed < 0) return false;
	if (_numt == 0) return false;

	length = _length;
	period = _period;
	wspeed = _wspeed;
	numt = _numt;

	courant = 0.950;

	dt = period / (double)numt;
	dx = wspeed * dt / courant;

	numx = (size_t)(length / dx);
	wave.resize(numt);
	for (auto& psi : wave)
	{
		psi.resize(numx);
	}

	return true;
}

bool FDTD::setBasicCondition(double _wspeed, double _length, double _period, size_t _numx, size_t _numt)
{
	if (_length < 0) return false;
	if (_period < 0) return false;
	if (_wspeed < 0) return false;
	if (_numx == 0) return false;
	if (_numt == 0) return false;

	length = _length;
	period = _period;
	wspeed = _wspeed;
	numx = _numx;
	numt = _numt;

	dx = length / (double)numx;
	dt = period / (double)numt;

	courant = wspeed * dt / dx;

	wave.resize(numt);
	for (auto& psi : wave)
	{
		psi.resize(numx);
	}

	if (courant < 1.0) return true;
	return false;
}

bool FDTD::setFixedEndCondition(std::vector<double>& _wave) const
{
	if (_wave.size() == 0) return false;

	_wave[0] = 0;
	_wave[_wave.size() - 1] = 0;
}

bool FDTD::setFreeEndCondition(std::vector<double>& _wave) const
{
	if (_wave.size() < 2) return false;

	_wave[0] = _wave[1];
	_wave[_wave.size() - 1] = _wave[_wave.size() - 2];
}

bool FDTD::setFreeFixedEndCondition(std::vector<double>& _wave) const
{
	if (_wave.size() < 2) return false;

	_wave[0] = _wave[1];
	_wave[_wave.size() - 1] = 0;
}

std::vector<double> FDTD::generateRandomCondition() const
{
	std::vector<double> ret(numx);

	for (size_t i = 1; i < numx - 1; ++i)
	{
		ret[i] = 1.0 - 2.0 * (double)rand() / (double)RAND_MAX;
	}

	return ret;
}

std::vector<double> FDTD::generateSinCondition(double _wlen, double _phase) const
{
	std::vector<double> ret(numx);

	for (size_t i = 1; i < numx - 1; ++i)
	{
		ret[i] = std::sin(2.0 * PI / _wlen * dx * (double)i + _phase);
	}

	return ret;
}

std::vector<double> FDTD::generateImpulseCondition(double _istar, double _iend) const
{
	assert(_istar > 0 && _istar < 1.0);
	assert(_iend > 0 && _iend < 1.0);
	assert(_istar < _iend);

	std::vector<double> ret(numx);

	for (size_t i = 1; i < numx - 1; ++i)
	{
		if ((double)numx * _istar < i && i < (double)numx * _iend)
		{
			ret[i] = 1.0;
		}
	}

	return ret;
}

WaveFunction FDTD::castWaveFunction(double _namp, double _inspt, unsigned int _srate, unsigned short _sbit)
{
	WaveFunction ret;
	WaveData dat;
	size_t idx = (size_t)((double)numx * _inspt);
	double ramp = _namp * pow(2.0, (double)_sbit - 1.0);

	assert(idx < numx);

	double sfreq = 1.0 / dt;
	double sinc = sfreq / (double)_srate;

	for (double i = 0; i < (double)numt; i = i + sinc)
	{
		dat.push_back((short)(ramp * wave[(size_t)i][idx]));
	}

	ret.setWaveFunction(dat, _srate, _sbit);

	return ret;
}
