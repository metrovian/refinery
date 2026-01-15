#include "abstract.h"

class pendulum_simple : public pendulum_abstract {
protected: /* constant */
	const double length_ = 1.000E+0;
	const double damp_ = 5.000E-1;

public: /* abstract */
	virtual void step(double tau) override final;
	virtual void reset(double theta, double omega) override final;
};