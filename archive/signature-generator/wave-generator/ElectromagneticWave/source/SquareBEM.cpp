#include "SquareBEM.h"
#include "Predefined.h"

SquareBEM::SquareBEM(double _lenx, double _leny, size_t _numx, size_t _numy)
{
	rada = 0.125 / kamp;
	posx = 0.5 * _lenx;
	posy = 0.5 * _leny;

	for (unsigned short i = 0; i < numa; ++i)
	{
		double phi = PI * 2.0 * static_cast<double>(i) / static_cast<double>(numa);

		double eosx = posx + rada * std::cos(phi);
		double eosy = posy + rada * std::sin(phi);

		setElement(Eigen::Vector2d(eosx, eosy), Eigen::Vector2d(std::cos(phi), std::sin(phi)));
	}

	solve(_lenx, _leny, _numx, _numy);
}

SquareBEM::SquareBEM(double _lenx, double _leny, size_t _numx, size_t _numy, double _kamp, double _famp)
{
	setPlaneWave(_kamp, _famp);

	rada = 0.125 / kamp;
	posx = 0.5 * _lenx;
	posy = 0.5 * _leny;

	for (unsigned short i = 0; i < numa; ++i)
	{
		double phi = PI * 2.0 * static_cast<double>(i) / static_cast<double>(numa);

		double eosx = posx + rada * std::cos(phi);
		double eosy = posy + rada * std::sin(phi);

		setElement(Eigen::Vector2d(eosx, eosy), Eigen::Vector2d(std::cos(phi), std::sin(phi)));
	}

	solve(_lenx, _leny, _numx, _numy);
}

SquareBEM::SquareBEM(double _lenx, double _leny, size_t _numx, size_t _numy, unsigned short _numa, double _rada, double _posx, double _posy)
{
	numa = _numa;

	rada = _rada;
	posx = _posx;
	posy = _posy;

	for (unsigned short i = 0; i < numa; ++i)
	{
		double phi = PI * 2.0 * static_cast<double>(i) / static_cast<double>(numa);

		double eosx = posx + rada * std::cos(phi);
		double eosy = posy + rada * std::sin(phi);

		setElement(Eigen::Vector2d(eosx, eosy), Eigen::Vector2d(std::cos(phi), std::sin(phi)));
	}

	solve(_lenx, _leny, _numx, _numy);
}

SquareBEM::SquareBEM(double _lenx, double _leny, size_t _numx, size_t _numy, unsigned short _numa, double _rada, double _posx, double _posy, double _kamp, double _famp)
{
	setPlaneWave(_kamp, _famp);

	numa = _numa;

	rada = _rada;
	posx = _posx;
	posy = _posy;

	for (unsigned short i = 0; i < numa; ++i)
	{
		double phi = PI * 2.0 * static_cast<double>(i) / static_cast<double>(numa);

		double eosx = posx + rada * std::cos(phi);
		double eosy = posy + rada * std::sin(phi);

		setElement(Eigen::Vector2d(eosx, eosy), Eigen::Vector2d(std::cos(phi), std::sin(phi)));
	}

	solve(_lenx, _leny, _numx, _numy);
}
