#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <memory>
#include <chrono>
#include <thread>
#include <grpcpp/grpcpp.h>

class pendulum_abstract {
protected: /* constant */
	const double gravity_ = 9.807E+0;
	const double dt_ = 1.000E-2;

protected: /* parameter */
	double time_ = 0.000E+0;
	double theta_ = 0.000E+0;
	double omega_ = 0.000E+0;

public: /* getter */
	double get_time();
	double get_theta();
	double get_omega();

public: /* abstract */
	virtual void step(double tau) = 0;
	virtual void reset(double theta, double omega) = 0;
};