#pragma once
#include "DDA.h"

class LineDDA : public DDA
{
private: /* config */
	unsigned char numd = 15;
	double lend;
	double posx;
	double posy;

public: /* constructor */
	LineDDA(double _lenx, double _leny, size_t _numx, size_t _numy);
	LineDDA(double _lenx, double _leny, size_t _numx, size_t _numy, double _kvex, double _kvey, double _famp);
	LineDDA(double _lenx, double _leny, size_t _numx, size_t _numy, unsigned char _numd, double _lend, double _posx, double _posy);
	LineDDA(double _lenx, double _leny, size_t _numx, size_t _numy, unsigned char _numd, double _lend, double _posx, double _posy, double _kvex, double _kvey, double _famp);
};