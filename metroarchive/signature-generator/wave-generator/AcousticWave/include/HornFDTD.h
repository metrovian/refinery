#pragma once
#include "FDTD.h"

class HornFDTD : public FDTD
{
private: /* parameters */
	double decay = 1.0;

public: /* constructor */
	HornFDTD(double _length, double _period, size_t _numt);
	HornFDTD(double _length, double _period, size_t _numt, double _sonic, double _decay);

protected: /* virtual */
	bool solve() override;
};