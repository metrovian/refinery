#pragma once
#include "Eigen/Dense"

class EigenMatrix
{
public: /* static */
	static Eigen::MatrixXd pinvSVD(const Eigen::MatrixXd& _mat);
	static Eigen::MatrixXd pinvMP(const Eigen::MatrixXd& _mat);
};