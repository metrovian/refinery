#pragma once
#include "WaveField.h"
#include "EigenMatrix.h"

class Element
{
public: /* data */
	Eigen::Vector2d pos = Eigen::Vector2d(0.0, 0.0);
	Eigen::Vector2d vec = Eigen::Vector2d(1.0, 0.0);

public: /* constructor */
	Element();
	Element(Eigen::Vector2d _pos, Eigen::Vector2d _vec);
};

typedef std::vector<Element> Antenna;
class BEM
{
protected: /* data */
	WaveField wave;
	Antenna data;

protected: /* source */
	double kamp = 10.0;
	double vamp = 1.0;

protected: /* condition */
	bool setElement();
	bool setElement(Eigen::Vector2d _pos, Eigen::Vector2d _vec);
	bool setPlaneWave(double _kamp, double _famp);

protected: /* parts */
	Eigen::MatrixXcd calcImpedanceMatrix() const;
	Eigen::VectorXcd calcExcitationVector() const;

public: /* viewer */
	bool render(unsigned int _width, unsigned int _height) const;

protected: /* solver */
	bool solve(double _lenx, double _leny, size_t _numx, size_t _numy);
};