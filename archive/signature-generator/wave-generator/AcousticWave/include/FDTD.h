#pragma once
#include "WaveFunction.h"

typedef std::vector<std::vector<double>> DataFDTD;
class FDTD
{
protected: /* data */
	DataFDTD wave;

protected: /* parameters */
	double length;
	double period;
	double wspeed;
	size_t numx;
	size_t numt;

protected: /* derived */
	double dx;
	double dt;
	double courant;

protected: /* condition */
	bool setBasicCondition(double _wspeed, double _length, double _period, size_t _numt);
	bool setBasicCondition(double _wspeed, double _length, double _period, size_t _numx, size_t _numt);

protected: /* boundary condition */
	bool setFixedEndCondition(std::vector<double>& _wave) const;
	bool setFreeEndCondition(std::vector<double>& _wave) const;
	bool setFreeFixedEndCondition(std::vector<double>& _wave) const;

protected: /* initial condition */
	std::vector<double> generateRandomCondition() const;
	std::vector<double> generateSinCondition(double _wlen, double _phase) const;
	std::vector<double> generateImpulseCondition(double _istar, double _iend) const;

public: /* public use */
	WaveFunction castWaveFunction(double _namp, double _inspt, unsigned int _srate, unsigned short _sbit);

protected: /* virtual */
	virtual bool solve() = 0;
};