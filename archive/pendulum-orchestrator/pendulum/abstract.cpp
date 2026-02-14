#include "abstract.h"

double pendulum_abstract::get_time() {
	return time_;
}

double pendulum_abstract::get_theta() {
	return theta_;
}

double pendulum_abstract::get_omega() {
	return omega_;
}