#include "BEM.h"
#include "WaveViewer.h"
#include "Predefined.h"

Element::Element()
{
	pos = Eigen::Vector2d(0.0, 0.0);
	vec = Eigen::Vector2d(1.0, 0.0);
}

Element::Element(Eigen::Vector2d _pos, Eigen::Vector2d _vec)
{
	pos = _pos;
	vec = _vec;
}

bool BEM::setElement()
{
	data.clear();
	return true;
}

bool BEM::setElement(Eigen::Vector2d _pos, Eigen::Vector2d _vec)
{
	data.push_back(Element(_pos, _vec));
	return true;
}

bool BEM::setPlaneWave(double _kamp, double _famp)
{
	if (_kamp < 0) return false;
	if (_famp < 0) return false;

	kamp = _kamp;
	vamp = _famp;

	return true;
}

Eigen::MatrixXcd BEM::calcImpedanceMatrix() const
{
	Eigen::MatrixXcd ret = Eigen::MatrixXcd(data.size(), data.size());

	for (size_t i = 0; i < data.size(); ++i)
	{
		for (size_t j = 0; j < data.size(); ++j)
		{
			if (i != j)
			{
				Eigen::Vector2d pvec = data[i].pos - data[j].pos;
				ret(i, j) = std::complex<double>(0.0, kamp / PI / 4.0) * data[i].vec.dot(data[j].vec) * std::exp(std::complex<double>(0.0, -kamp * pvec.norm())) / pvec.norm();
			}
			
			else
			{
				ret(i, j) = 0.0;
			}
		}
	}

	return ret;
}

Eigen::VectorXcd BEM::calcExcitationVector() const
{
	Eigen::VectorXcd ret = Eigen::VectorXcd(data.size());
	ret(data.size() / 2) = vamp;

	return ret;
}

bool BEM::render(unsigned int _width, unsigned int _height) const
{
	WaveViewer viewer(_width, _height);
	viewer.display(wave);

	return true;
}

bool BEM::solve(double _lenx, double _leny, size_t _numx, size_t _numy)
{
	if (_lenx == 0) return false;
	if (_leny == 0) return false;
	if (_numx == 0) return false;
	if (_numy == 0) return false;

	WaveField ret(MODE::TRANSVERSE_MAGNETIC, _lenx, _leny, _numx, _numy);
	Eigen::VectorXcd cur = EigenMatrix::pinvSVD(calcImpedanceMatrix()) * calcExcitationVector();

	for (size_t i = 0; i < _numx; ++i)
	{
		for (size_t j = 0; j < _numy; ++j)
		{
			std::complex<double> res = 0;

			for (size_t k = 0; k < data.size(); ++k)
			{
				Eigen::Vector2d pos = Eigen::Vector2d(ret.getDX() * static_cast<double>(i), ret.getDY() * static_cast<double>(j));

				Eigen::Vector2d pvec = pos - data[k].pos;
				Eigen::Vector2cd jvec = cur(k) * data[k].vec;

				if (pvec.norm() > 0) res += std::complex<double>(0.0, kamp / PI / 4.0) * (pvec(0) * jvec(1) - pvec(1) * jvec(0)) * std::exp(std::complex<double>(0.0, -kamp * pvec.norm())) / pvec.norm();
			}

			ret.setField(Eigen::Vector3d(0, 0, res.real()), i, j);
		}
	}

	wave = ret;

	return true;
}
