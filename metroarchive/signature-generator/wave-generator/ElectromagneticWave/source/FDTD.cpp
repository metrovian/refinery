#include "FDTD.h"
#include "WaveViewer.h"
#include "Predefined.h"

bool FDTD::setBasicCondition(const WaveField& _init, double _period)
{
	if (_period < 0) return false;

	wave.push_back(_init);

	courant = 0.950;
	period = _period;

	dx = _init.getDX();
	dy = _init.getDY();

	numx = _init.getNX();
	numy = _init.getNY();

	dt = std::min(dx / C * courant, dy / C * courant) / sqrt(2);
	numt = static_cast<size_t>(period / dt);

	medium = generateVacuumMedium(numx, numy);

	return true;
}

bool FDTD::setBasicCondition(const WaveField& _init, double _period, size_t _numt)
{
	if (_period < 0) return false;
	if (_numt < 0) return false;

	wave.push_back(_init);
	
	numx = _init.getNX();
	numy = _init.getNY();
	numt = _numt;

	dx = _init.getDX();
	dy = _init.getDY();
	dt = _period / (double)_numt;

	courant = C * dt * std::sqrt(1.0 / dx / dx + 1.0 / dy / dy);
	period = _period;
	
	medium = generateVacuumMedium(numx, numy);

	if (courant < 1.0) return true;
	return false;
}

bool FDTD::setBasicCondition(const WaveField& _init, const SpaceField& _medium, double _period)
{
	if (_period < 0) return false;

	wave.push_back(_init);

	courant = 0.950;
	period = _period;

	dx = _init.getDX();
	dy = _init.getDY();

	numx = _init.getNX();
	numy = _init.getNY();

	dt = std::min(dx / C * courant, dy / C * courant) / sqrt(2);
	numt = static_cast<size_t>(period / dt);

	medium = _medium;

	return true;
}

bool FDTD::setBasicCondition(const WaveField& _init, const SpaceField& _medium, double _period, size_t _numt)
{
	if (_period < 0) return false;
	if (_numt < 0) return false;

	wave.push_back(_init);

	numx = _init.getNX();
	numy = _init.getNY();
	numt = _numt;

	dx = _init.getDX();
	dy = _init.getDY();
	dt = _period / (double)_numt;

	courant = C * dt * std::sqrt(1.0 / dx / dx + 1.0 / dy / dy);
	period = _period;

	medium = _medium;

	if (courant < 1.0) return true;
	return false;
}

Eigen::Vector3d FDTD::calcFreeEndBoundary(const WaveField& _field, size_t _idx, size_t _jdx) const
{
	if (_idx >= numx) return _field.getField(numx - 1, _jdx);
	if (_jdx >= numy) return _field.getField(_idx, numy - 1);
	if (_idx < 0) return _field.getField(0, _jdx);
	if (_jdx < 0) return _field.getField(_idx, 0);

	return _field.getField(_idx, _jdx);
}

Eigen::Vector3d FDTD::calcFixedEndBoundary(const WaveField& _field, size_t _idx, size_t _jdx) const
{
	if (_idx >= numx) return Eigen::Vector3d::Zero();
	if (_jdx >= numy) return Eigen::Vector3d::Zero();
	if (_idx <= 0) return _field.getField(1, _jdx);
	if (_jdx <= 0) return _field.getField(_idx, 1);

	return _field.getField(_idx, _jdx);
}

SpaceField FDTD::generateVacuumMedium(size_t _numx, size_t _numy)
{
	SpaceField ret;
	ret.resize(_numx);

	for (size_t i = 0; i < _numx; ++i)
	{
		ret[i].resize(_numy);

		for (size_t j = 0; j < _numy; ++j)
		{
			ret[i][j] = 1.0;
		}
	}

	return ret;
}

SpaceField FDTD::generateWaveguideMedium(double _lenx, double _leny, size_t _numx, size_t _numy, double _pos1, double _pos2, double _epr1, double _epr2)
{
	assert(_pos1 < _pos2);

	SpaceField ret;
	ret.resize(_numx);

	size_t cond1 = static_cast<size_t>((double)_numy / _leny * _pos1);
	size_t cond2 = static_cast<size_t>((double)_numy / _leny * _pos2);

	for (size_t i = 0; i < _numx; ++i)
	{
		ret[i].resize(_numy);

		for (size_t j = 0; j < _numy; ++j)
		{
			if (j > cond1 && j < cond2)
			{
				ret[i][j] = _epr2;
			}

			else
			{
				ret[i][j] = _epr1;
			}
		}
	}

	return ret;
}

SpaceField FDTD::generatePeriodicMedium(double _lenx, double _leny, size_t _numx, size_t _numy, double _pos1, double _pos2, double _thk1, double _thk2, double _epr1, double _epr2)
{
	assert(_pos1 < _pos2);

	SpaceField ret;
	ret.resize(_numx);

	size_t cond1 = static_cast<size_t>((double)_numy / _leny * _pos1);
	size_t cond2 = static_cast<size_t>((double)_numy / _leny * _pos2);

	for (size_t i = 0; i < _numx; ++i)
	{
		ret[i].resize(_numy);

		size_t rep = 1;

		for (size_t j = 0; j < _numy; ++j)
		{
			if (j > cond1 && j < cond2)
			{
				double posn = _leny * static_cast<double>(j) / static_cast<double>(_numy);
				double thkn = _thk1 + _thk2;

				if (posn >= _pos1 + static_cast<double>(rep) * thkn)
				{
					rep++;
				}

				if (posn < _pos1 + static_cast<double>(rep) * thkn - _thk2)
				{
					ret[i][j] = _epr1;
				}

				else if (posn < _pos1 + static_cast<double>(rep) * thkn)
				{
					ret[i][j] = _epr2;
				}
			}

			else
			{
				ret[i][j] = 1.0;
			}
		}
	}

	return ret;
}

WaveField FDTD::generateHuygensSource(MODE _mode, double _lenx, double _leny, size_t _numx, size_t _numy, double _posx, double _posy, double _famp)
{
	WaveField ret(_mode, _lenx, _leny, _numx, _numy);

	size_t idx = static_cast<size_t>(_numx * (_posx / _lenx));
	size_t jdx = static_cast<size_t>(_numy * (_posy / _leny));

	if (idx < 0) return ret;
	if (jdx < 0) return ret;
	if (idx >= _numx) return ret;
	if (jdx >= _numy) return ret;

	ret.setField(Eigen::Vector3d(0, 0, _famp), idx, jdx);

	return ret;
}

WaveField FDTD::generateHuygensSource(MODE _mode, double _lenx, double _leny, size_t _numx, size_t _numy, double _posx, double _posy, double _time, double _famp, double _freq)
{
	WaveField ret(_mode, _lenx, _leny, _numx, _numy);

	size_t idx = static_cast<size_t>(_numx * (_posx / _lenx));
	size_t jdx = static_cast<size_t>(_numy * (_posy / _leny));

	if (idx < 0) return ret;
	if (jdx < 0) return ret;
	if (idx >= _numx) return ret;
	if (jdx >= _numy) return ret;

	ret.setField(Eigen::Vector3d(0, 0, _famp * std::sin(2.0 * PI * _freq * _time)), idx, jdx);

	return ret;
}

double FDTD::calcElapsedTime() const
{
	return static_cast<double>(wave.size()) * dt;
}

bool FDTD::render(unsigned int _width, unsigned int _height) const
{
	if (wave.size() == 0) return false;

	WaveViewer viewer(_width, _height);
	viewer.display(wave);

	return true;
}

WaveField FDTD::calcNextStepField(const WaveField& _now) const
{
	return calcNextStepField(_now, std::bind(&FDTD::calcFixedEndBoundary, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

WaveField FDTD::calcNextStepField(const WaveField& _now, const std::function<Eigen::Vector3d(const WaveField& _field, size_t _idx, size_t _jdx)>& _inspect) const
{
	WaveField next = _now;

	double estp = dt / EP;
	double mstp = dt / MU;

	if (mode == MODE::TRANSVERSE_ELECTRIC)
	{
		for (size_t i = 0; i < numx; ++i)
		{
			for (size_t j = 0; j < numy; ++j)
			{
				Eigen::Vector3d now = _inspect(next, i, j);
				Eigen::Vector3d ndx = _inspect(next, i - 1, j);
				Eigen::Vector3d ndy = _inspect(next, i, j - 1);

				if (i < PMLW) now.z() -= PMLS * now.z();
				if (j < PMLW) now.z() -= PMLS * now.z();
				if (i >= numx - PMLW) now.z() -= PMLS * now.z();
				if (j >= numy - PMLW) now.z() -= PMLS * now.z();

				now.z() += estp * ((now.y() - ndx.y()) / dx - (now.x() - ndy.x()) / dy) / medium[i][j];

				next.setField(now, i, j);
			}
		}

		for (size_t i = 0; i < numx; ++i)
		{
			for (size_t j = 0; j < numy; ++j)
			{
				Eigen::Vector3d now = _inspect(next, i, j);
				Eigen::Vector3d ndx = _inspect(next, i + 1, j);
				Eigen::Vector3d ndy = _inspect(next, i, j + 1);

				now.x() -= mstp * (ndy.z() - now.z()) / dy;
				now.y() += mstp * (ndx.z() - now.z()) / dx;

				next.setField(now, i, j);
			}
		}
	}

	else if (mode == MODE::TRANSVERSE_MAGNETIC)
	{
		for (size_t i = 0; i < numx; ++i)
		{
			for (size_t j = 0; j < numy; ++j)
			{
				Eigen::Vector3d now = _inspect(next, i, j);
				Eigen::Vector3d ndx = _inspect(next, i - 1, j);
				Eigen::Vector3d ndy = _inspect(next, i, j - 1);

				if (i < PMLW) now.z() -= PMLS * now.z();
				if (j < PMLW) now.z() -= PMLS * now.z();
				if (i >= numx - PMLW) now.z() -= PMLS * now.z();
				if (j >= numy - PMLW) now.z() -= PMLS * now.z();

				now.z() -= mstp * ((now.y() - ndx.y()) / dx - (now.x() - ndy.x()) / dy);

				next.setField(now, i, j);
			}
		}

		for (size_t i = 0; i < numx; ++i)
		{
			for (size_t j = 0; j < numy; ++j)
			{
				Eigen::Vector3d now = _inspect(next, i, j);
				Eigen::Vector3d ndx = _inspect(next, i + 1, j);
				Eigen::Vector3d ndy = _inspect(next, i, j + 1);

				now.x() += estp * (ndy.z() - now.z()) / dy / medium[i][j];
				now.y() -= estp * (ndx.z() - now.z()) / dx / medium[i][j];

				next.setField(now, i, j);
			}
		}
	}

	return next;
}
