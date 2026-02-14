#pragma once
#include "FDTD.h"

class PlanarWaveguideFDTD : public FDTD
{
private: /* data */
	double epr1 = 1.00;
	double epr2 = 1.50;
	double pos1;
	double pos2;
	double famp = 1.00;
	double freq = 1.0E+8;

public: /* constructor */
	PlanarWaveguideFDTD(MODE _mode, double _lenx, double _leny, double _period, size_t _numx, size_t _numy);
	PlanarWaveguideFDTD(MODE _mode, double _lenx, double _leny, double _period, size_t _numx, size_t _numy, double _pos1, double _pos2, double _epr1, double _epr2, double _famp, double _freq);
	PlanarWaveguideFDTD(MODE _mode, double _lenx, double _leny, double _period, size_t _numx, size_t _numy, size_t _numt);
	PlanarWaveguideFDTD(MODE _mode, double _lenx, double _leny, double _period, size_t _numx, size_t _numy, size_t _numt, double _pos1, double _pos2, double _epr1, double _epr2, double _famp, double _freq);

protected: /* virtual */
	bool solve(MODE _mode, double _lenx, double _leny, double _period, size_t _numx, size_t _numy) override;
	bool solve(MODE _mode, double _lenx, double _leny, double _period, size_t _numx, size_t _numy, size_t _numt) override;
};
