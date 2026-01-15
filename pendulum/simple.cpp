#include "simple.h"

void pendulum_simple::step(double tau) {
	double gravity = gravity_ / length_ * std::sin(theta_);
	double damp = damp_ * omega_;
	time_ += dt_;
	theta_ += omega_ * dt_;
	omega_ += (tau - damp - gravity) * dt_;
	return;
}

void pendulum_simple::reset(double theta, double omega) {
	theta_ = theta;
	omega_ = omega;
	time_ = 0.000E+0;
	return;
}