#pragma once
#include "WaveField.h"

typedef std::vector<WaveField> TimeField;
typedef std::vector<std::vector<double>> SpaceField;
class FDTD
{
protected: /* data */
	TimeField wave;
	SpaceField medium;
	MODE mode;

protected: /* config */
	double lenx;
	double leny;
	double period;
	size_t numx;
	size_t numy;
	size_t numt;
	
protected: /* derived */
	double dx;
	double dy;
	double dt;
	double courant;

protected: /* condition */
	bool setBasicCondition(const WaveField& _init, double _period);
	bool setBasicCondition(const WaveField& _init, double _period, size_t _numt);
	bool setBasicCondition(const WaveField& _init, const SpaceField& _medium, double _period);
	bool setBasicCondition(const WaveField& _init, const SpaceField& _medium, double _period, size_t _numt);

protected: /* inspect */
	Eigen::Vector3d calcFreeEndBoundary(const WaveField& _field, size_t _idx, size_t _jdx) const;
	Eigen::Vector3d calcFixedEndBoundary(const WaveField& _field, size_t _idx, size_t _jdx) const;

protected: /* medium */
	static SpaceField generateVacuumMedium(size_t _numx, size_t _numy);
	static SpaceField generateWaveguideMedium(double _lenx, double _leny, size_t _numx, size_t _numy, double _pos1, double _pos2, double _epr1, double _epr2);
	static SpaceField generatePeriodicMedium(double _lenx, double _leny, size_t _numx, size_t _numy, double _pos1, double _pos2, double _thk1, double _thk2, double _epr1, double _epr2);

protected: /* source */
	static WaveField generateHuygensSource(MODE _mode, double _lenx, double _leny, size_t _numx, size_t _numy, double _posx, double _posy, double _famp);
	static WaveField generateHuygensSource(MODE _mode, double _lenx, double _leny, size_t _numx, size_t _numy, double _posx, double _posy, double _time, double _famp, double _freq);

protected: /* parts */
	double calcElapsedTime() const;

public: /* viewer */
	bool render(unsigned int _width, unsigned int _height) const;

protected: /* update */
	WaveField calcNextStepField(const WaveField& _now) const;
	WaveField calcNextStepField(const WaveField& _now, const std::function<Eigen::Vector3d(const WaveField& _field, size_t _idx, size_t _jdx)>& _inspect) const;

protected: /* virtual */
	virtual bool solve(MODE _mode, double _lenx, double _leny, double _period, size_t _numx, size_t _numy) = 0;
	virtual bool solve(MODE _mode, double _lenx, double _leny, double _period, size_t _numx, size_t _numy, size_t _numt) = 0;
};