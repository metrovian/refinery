#pragma once
#include "DigitalWaveguide.h"

class PluckStringKS : public DigitalWaveguide
{
private: /* data */
	unsigned char pow = 2;

public: /* constructor */
	PluckStringKS(double _namp, double _freq, double _dura, unsigned int _srate, unsigned short _sbit);
	PluckStringKS(double _namp, double _freq, double _dura, unsigned int _srate, unsigned short _sbit, unsigned char _pow);

protected: /* virtual */
	bool synthesis(double _namp, double _freq, double _dura, unsigned int _srate, unsigned short _sbit) override;
};

class PluckStringEKS : public DigitalWaveguide
{
private: /* data */
	double decay = 15.000;
	double damp = 0.500;
	double tune = 0.100;
	double modulus = 0.850;
	double pos = 0.100;

public: /* constructor */
	PluckStringEKS(double _namp, double _freq, double _dura, unsigned int _srate, unsigned short _sbit);
	PluckStringEKS(double _namp, double _freq, double _dura, unsigned int _srate, unsigned short _sbit, double _decay, double _damp, double _tune, double _mod, double _pos);

protected: /* virtual */
	bool synthesis(double _namp, double _freq, double _dura, unsigned int _srate, unsigned short _sbit) override;
};