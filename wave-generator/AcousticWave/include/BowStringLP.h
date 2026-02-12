#pragma once
#include "DigitalWaveguide.h"

class BowStringLP : public DigitalWaveguide
{
private: /* data */
	unsigned char band = 10;
	unsigned char pnum = 40;
	double modulus = 0.100;
	double pos = 0.200;
	double decay = 0.300;
	double bwmax = 0.150;
	double bwrel = 0.950;

private: /* bow */
	double linearBow(double _decay, double _bwmax, double _bwrel, double _rtime) const;

public: /* constructor */
	BowStringLP(double _namp, double _freq, double _dura, unsigned int _srate, unsigned short _sbit);
	BowStringLP(double _namp, double _freq, double _dura, unsigned int _srate, unsigned short _sbit, unsigned char _band, unsigned char _pnum, double _mod, double _pos, double _decay, double _bwmax, double _bwrel);

protected: /* virtual */
	bool synthesis(double _namp, double _freq, double _dura, unsigned int _srate, unsigned short _sbit) override;
};