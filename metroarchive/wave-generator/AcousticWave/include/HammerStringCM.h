#pragma once
#include "DigitalWaveguide.h"

class HammerStringCM : public DigitalWaveguide
{
private: /* data */
	double imps[3] = { 0.200, 0.600, 0.800 };
	double impe[3] = { 0.250, 0.650, 0.850 };
	double modulus[3] = { 0.950, 0.850, 0.650 };
	double invp = 1.500;
	double decay = 4.500;

public: /* constructor */
	HammerStringCM(double _namp, double _freq, double _dura, unsigned int _srate, unsigned short _sbit);
	HammerStringCM(double _namp, double _freq, double _dura, unsigned int _srate, unsigned short _sbit, double _decay, double _mod[3], double _imps[3], double _impe[3]);

protected: /* protected */
	bool synthesis(double _namp, double _freq, double _dura, unsigned int _srate, unsigned short _sbit) override;
};