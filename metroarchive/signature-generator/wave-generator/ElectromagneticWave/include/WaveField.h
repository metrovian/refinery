#pragma once
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <thread>
#include <vector>
#include <Eigen/Dense>

enum class MODE
{
	TRANSVERSE_ELECTRIC,
	TRANSVERSE_MAGNETIC,
};

typedef std::vector<std::vector<Eigen::Vector3d>> WaveData;
class WaveField
{
private: /* data */
	WaveData field;
	MODE mode;

private: /* config */
	double lenx;
	double leny;
	size_t numx;
	size_t numy;

private: /* derived */
	double dx;
	double dy;

private: /* status */
	bool isvalid = false;

public: /* constructor */
	WaveField() = default;
	WaveField(MODE _mode, double _lenx, double _leny, size_t _numx, size_t _numy);

private: /* memory */
	bool mallocWaveField();
	bool check(size_t _idx, size_t _idy) const;
	bool check(const WaveField& _rhs) const;

public: /* operator */
	WaveField operator+(const WaveField& _rhs) const;
	WaveField operator-(const WaveField& _rhs) const;

public: /* public use */
	bool setField(Eigen::Vector3d _field, size_t _idx, size_t _idy);
	
	Eigen::Vector3d getField(size_t _idx, size_t _idy) const;
	Eigen::Vector3d getPoyntingVector(size_t _idx, size_t _idy) const;

	double getDX() const;
	double getDY() const;
	double getLX() const;
	double getLY() const;

	size_t getNX() const;;
	size_t getNY() const;;
};