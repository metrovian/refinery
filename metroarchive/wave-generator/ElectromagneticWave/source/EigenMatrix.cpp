#include "EigenMatrix.h"

Eigen::MatrixXcd EigenMatrix::pinvSVD(const Eigen::MatrixXcd& _mat)
{
	Eigen::JacobiSVD<Eigen::MatrixXcd> svd(_mat, Eigen::ComputeFullU | Eigen::ComputeFullV);

	Eigen::MatrixXcd sinv(_mat.cols(), _mat.rows());
	sinv.setZero();

	const auto& singular = svd.singularValues();

	for (unsigned int i = 0; i < singular.size(); ++i)
	{
		if (singular(i) > 1.0E-10)
		{
			sinv(i, i) = 1.0 / singular(i);
		}
	}

	return svd.matrixV() * sinv * svd.matrixU().transpose();
}

Eigen::MatrixXcd EigenMatrix::pinvMP(const Eigen::MatrixXcd& _mat)
{
	Eigen::MatrixXcd ret;

	if (_mat.rows() > _mat.cols())
	{
		ret = (_mat.transpose() * _mat).inverse() * _mat.transpose();
	}

	else
	{
		ret = _mat.transpose() * (_mat * _mat.transpose()).inverse();
	}

	return ret;
}
