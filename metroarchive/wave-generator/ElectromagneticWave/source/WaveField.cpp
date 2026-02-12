#include "WaveField.h"

WaveField::WaveField(MODE _mode, double _lenx, double _leny, size_t _numx, size_t _numy)
{
	mode = _mode;
	numx = _numx;
	numy = _numy;
	dx = _lenx / (double)_numx;
	dy = _leny / (double)_numy;

	isvalid = mallocWaveField();
}

bool WaveField::mallocWaveField()
{
	field.resize(numx);

	for (size_t i = 0; i < numx; ++i)
	{
		field[i].resize(numy);

		for (size_t j = 0; j < numy; ++j)
		{
			field[i][j] = Eigen::Vector3d(0, 0, 0);
		}
	}

	return true;
}

bool WaveField::check(size_t _idx, size_t _idy) const
{
	if (_idx >= numx) return false;
	if (_idy >= numy) return false;

	return isvalid;
}

bool WaveField::check(const WaveField& _rhs) const
{
	if (numx != _rhs.numx) return false;
	if (numy != _rhs.numy) return false;

	return true;
}

WaveField WaveField::operator+(const WaveField& _rhs) const
{
	WaveField ret = *this;

	if (check(_rhs))
	{
		for (size_t i = 0; i < numx; ++i)
		{
			for (size_t j = 0; j < numy; ++j)
			{
				ret.setField(getField(i, j) + _rhs.getField(i, j), i, j);
			}
		}
	}

	return ret;
}

WaveField WaveField::operator-(const WaveField& _rhs) const
{
	WaveField ret = *this;

	if (check(_rhs))
	{
		for (size_t i = 0; i < numx; ++i)
		{
			for (size_t j = 0; j < numy; ++j)
			{
				ret.setField(getField(i, j) - _rhs.getField(i, j), i, j);
			}
		}
	}

	return ret;
}

bool WaveField::setField(Eigen::Vector3d _field, size_t _idx, size_t _idy)
{
	if (!check(_idx, _idy)) return false;

	field[_idx][_idy] = _field;
	return true;
}

Eigen::Vector3d WaveField::getField(size_t _idx, size_t _idy) const
{
	if (!check(_idx, _idy)) return Eigen::Vector3d(NAN, NAN, NAN);
	return field[_idx][_idy];
}

Eigen::Vector3d WaveField::getPoyntingVector(size_t _idx, size_t _idy) const
{
	if (!check(_idx, _idy)) return Eigen::Vector3d(NAN, NAN, NAN);

	Eigen::Vector3d electric = Eigen::Vector3d::Zero();
	Eigen::Vector3d magnetic = Eigen::Vector3d::Zero();

	if (mode == MODE::TRANSVERSE_ELECTRIC)
	{
		magnetic.head<2>() = field[_idx][_idy].head<2>();
		electric.z() = field[_idx][_idy].z();
	}

	else if (mode == MODE::TRANSVERSE_MAGNETIC)
	{
		electric.head<2>() = field[_idx][_idy].head<2>();
		magnetic.z() = field[_idx][_idy].z();
	}

	return electric.cross(magnetic);
}

double WaveField::getDX() const 
{ 
	return dx; 
}

double WaveField::getDY() const 
{ 
	return dy; 
}

double WaveField::getLX() const 
{ 
	return lenx; 
}

double WaveField::getLY() const 
{ 
	return leny; 
}

size_t WaveField::getNX() const 
{ 
	return numx; 
}

size_t WaveField::getNY() const 
{ 
	return numy; 
}
