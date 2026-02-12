#pragma once
#include "Eigen/Dense"

class EigenMatrix
{
public: /* static */
	static Eigen::MatrixXcd pinvSVD(const Eigen::MatrixXcd& _mat);
	static Eigen::MatrixXcd pinvMP(const Eigen::MatrixXcd& _mat);
};
