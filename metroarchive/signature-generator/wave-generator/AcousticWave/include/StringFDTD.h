#pragma once
#include "FDTD.h"

class StringFDTD : public FDTD
{
private: /* parameters */
	double decay = 5.0;

public: /* constructor */
	StringFDTD(double _length, double _period, size_t _numt, double _tension, double _density);
	StringFDTD(double _length, double _period, size_t _numt, double _tension, double _density, double _decay);

protected: /* public use */
	bool solve() override;
};