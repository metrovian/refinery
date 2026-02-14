#pragma once
#include "FDTD.h"

class PipeFDTD : public FDTD
{
private: /* parameters */
	double decay = 1.0;

public: /* constructor */
	PipeFDTD(double _length, double _period, size_t _numt);
	PipeFDTD(double _length, double _period, size_t _numt, double _sonic, double _decay);

protected: /* virtual */
	bool solve() override;
};