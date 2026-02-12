#pragma once
#include "WaveField.h"

class Dipole
{
public: /* data */
	Eigen::Vector2d pos = Eigen::Vector2d::Zero();
	double alpha = 1.0;

public: /* constructor */
	Dipole();
	Dipole(Eigen::Vector2d _pos);
	Dipole(Eigen::Vector2d _pos, double _alpha);
};

typedef std::vector<Dipole> Dipoles;
class DDA
{
protected: /* data */
	WaveField wave;
	Dipoles data;

protected: /* source */
	Eigen::Vector2d kvec = Eigen::Vector2d(10.0, 0);
	double famp = 1.0;

protected: /* condition */
	bool setDipole();
	bool setDipole(Eigen::Vector2d _pos);
	bool setDipole(Eigen::Vector2d _pos, double _alpha);
	bool setPlaneWave(Eigen::Vector2d _kvec, double _famp);

protected: /* parts */
	Eigen::VectorXcd calcPlaneWave() const;
	Eigen::VectorXcd calcPolarization() const;

public: /* viewer */
	bool render(unsigned int _width, unsigned int _height) const;

protected: /* solver */
	bool solve(double _lenx, double _leny, size_t _numx, size_t _numy);
};