#pragma once
#include "BEM.h"

class SquareBEM : public BEM
{
private: /* config */
	unsigned short numa = 100;
	double rada;
	double posx;
	double posy;

public: /* constructor */
	SquareBEM(double _lenx, double _leny, size_t _numx, size_t _numy);
	SquareBEM(double _lenx, double _leny, size_t _numx, size_t _numy, double _kamp, double _famp);
	SquareBEM(double _lenx, double _leny, size_t _numx, size_t _numy, unsigned short _numa, double _rada, double _posx, double _posy);
	SquareBEM(double _lenx, double _leny, size_t _numx, size_t _numy, unsigned short _numa, double _rada, double _posx, double _posy, double _kamp, double _famp);
};