#include "DDA.h"
#include "WaveViewer.h"

Dipole::Dipole()
{
	pos = Eigen::Vector2d::Zero();
	alpha = 1.0;
}

Dipole::Dipole(Eigen::Vector2d _pos)
{
	pos = _pos;
	alpha = 1.0;
}

Dipole::Dipole(Eigen::Vector2d _pos, double _alpha)
{
	pos = _pos;
	alpha = _alpha;
}

bool DDA::setDipole()
{
	data.clear();
	return true;
}

bool DDA::setDipole(Eigen::Vector2d _pos)
{
	data.push_back(Dipole(_pos));
	return true;
}

bool DDA::setDipole(Eigen::Vector2d _pos, double _alpha)
{
	if (_alpha < 0) return false;

	data.push_back(Dipole(_pos, _alpha));
	return true;
}

bool DDA::setPlaneWave(Eigen::Vector2d _kvec, double _famp)
{
	if (_kvec.norm() == 0) return false;
	if (_famp < 0) return false;

	kvec = _kvec;
	famp = _famp;

	return true;
}

Eigen::VectorXcd DDA::calcPlaneWave() const
{
	Eigen::VectorXcd ret(data.size());

	for (size_t i = 0; i < data.size(); ++i)
	{
		ret(i) = famp * std::exp(std::complex<double>(0, data[i].pos.dot(kvec)));
	}

	return ret;
}

Eigen::VectorXcd DDA::calcPolarization() const
{
	Eigen::MatrixXcd mat(data.size(), data.size());

	for (size_t i = 0; i < data.size(); ++i)
	{
		for (size_t j = 0; j < data.size(); ++j)
		{
			if (i != j)
			{
				double arg = kvec.norm() * (data[i].pos - data[j].pos).norm();
				mat(i, j) = std::complex<double>(0, 0.25) * std::complex<double>(std::cyl_bessel_j(0, arg), std::cyl_neumann(0, arg));
			}

			else
			{
				mat(i, j) = 1.0 / data[i].alpha;
			}
		}
	}

	Eigen::JacobiSVD<Eigen::MatrixXcd> svd(mat, Eigen::ComputeFullU | Eigen::ComputeFullV);
	Eigen::VectorXcd ret = svd.solve(calcPlaneWave());

	return ret;
}

bool DDA::render(unsigned int _width, unsigned int _height) const
{
	WaveViewer viewer(_width, _height);
	viewer.display(wave);

	return true;
}

bool DDA::solve(double _lenx, double _leny, size_t _numx, size_t _numy)
{
	if (_lenx == 0) return false;
	if (_leny == 0) return false;
	if (_numx == 0) return false;
	if (_numy == 0) return false;

	WaveField ret(MODE::TRANSVERSE_ELECTRIC, _lenx, _leny, _numx, _numy);

	for (size_t i = 0; i < _numx; ++i)
	{
		for (size_t j = 0; j < _numy; ++j)
		{
			double rx = static_cast<double>(i) * _lenx / static_cast<double>(_numx);
			double ry = static_cast<double>(j) * _leny / static_cast<double>(_numy);

			Eigen::Vector3d res = Eigen::Vector3d::Zero();
			Eigen::Vector2d pos = Eigen::Vector2d(rx, ry);
			Eigen::VectorXcd pvec = calcPolarization();

			for (size_t k = 0; k < data.size(); ++k)
			{
				double arg = kvec.norm() * (pos - data[k].pos).norm();
				if (arg > 0) res.z() += (pvec[k] * std::complex<double>(0, 0.25) * std::complex<double>(std::cyl_bessel_j(0, arg), std::cyl_neumann(0, arg))).real();
			}

			ret.setField(res, i, j);
		}
	}

	wave = ret;

	return true;
}
