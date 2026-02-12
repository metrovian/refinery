#include "LineDDA.h"

LineDDA::LineDDA(double _lenx, double _leny, size_t _numx, size_t _numy)
{
	setPlaneWave(Eigen::Vector2d(5.0, 5.0), 1.0);
	setDipole();

	lend = _lenx / 50.0;

	posx = (_lenx - lend) / 2.0;
	posy = _leny / 2.0;

	double dl = lend / static_cast<double>(numd);

	for (unsigned char i = 0; i < numd; ++i)
	{
		setDipole(Eigen::Vector2d(posx + static_cast<double>(i) * dl, posy));
	}

	solve(_lenx, _leny, _numx, _numy);
}

LineDDA::LineDDA(double _lenx, double _leny, size_t _numx, size_t _numy, double _kvex, double _kvey, double _famp)
{
	setPlaneWave(Eigen::Vector2d(_kvex, _kvey), _famp);
	setDipole();

	lend = _lenx / 50.0;

	posx = (_lenx - lend) / 2.0;
	posy = _leny / 2.0;

	double dl = lend / static_cast<double>(numd);

	for (unsigned char i = 0; i < numd; ++i)
	{
		setDipole(Eigen::Vector2d(posx + static_cast<double>(i) * dl, posy));
	}

	solve(_lenx, _leny, _numx, _numy);
}

LineDDA::LineDDA(double _lenx, double _leny, size_t _numx, size_t _numy, unsigned char _numd, double _lend, double _posx, double _posy)
{
	setPlaneWave(Eigen::Vector2d(5.0, 5.0), 1.0);
	setDipole();

	numd = _numd;
	lend = _lend;

	posx = _posx;
	posy = _posy;

	double dl = lend / static_cast<double>(numd);

	for (unsigned char i = 0; i < numd; ++i)
	{
		setDipole(Eigen::Vector2d(posx + static_cast<double>(i) * dl, posy));
	}

	solve(_lenx, _leny, _numx, _numy);
}

LineDDA::LineDDA(double _lenx, double _leny, size_t _numx, size_t _numy, unsigned char _numd, double _lend, double _posx, double _posy, double _kvex, double _kvey, double _famp)
{
	setPlaneWave(Eigen::Vector2d(_kvex, _kvey), _famp);
	setDipole();

	numd = _numd;
	lend = _lend;

	posx = _posx;
	posy = _posy;

	double dl = lend / static_cast<double>(numd);

	for (unsigned char i = 0; i < numd; ++i)
	{
		setDipole(Eigen::Vector2d(posx + static_cast<double>(i) * dl, posy));
	}

	solve(_lenx, _leny, _numx, _numy);
}
