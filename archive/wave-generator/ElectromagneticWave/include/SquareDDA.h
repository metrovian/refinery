#pragma once
#include "DDA.h"

class SquareDDA : public DDA
{
private: /* config */
	unsigned char numd = 4;
	double lend;
	double posx;
	double posy;

public: /* constructor */
	SquareDDA(double _lenx, double _leny, size_t _numx, size_t _numy);
	SquareDDA(double _lenx, double _leny, size_t _numx, size_t _numy, double _kvex, double _kvey, double _famp);
	SquareDDA(double _lenx, double _leny, size_t _numx, size_t _numy, unsigned char _numd, double _lend, double _posx, double _posy);
	SquareDDA(double _lenx, double _leny, size_t _numx, size_t _numy, unsigned char _numd, double _lend, double _posx, double _posy, double _kvex, double _kvey, double _famp);
};