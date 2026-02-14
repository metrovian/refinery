#include "SquareDDA.h"

SquareDDA::SquareDDA(double _lenx, double _leny, size_t _numx, size_t _numy)
{
	setDipole();

	lend = std::min(_lenx / 50.0, _leny / 50.0);

	posx = (_lenx - lend) / 2.0;
	posy = (_leny - lend) / 2.0;

	double dl = lend / static_cast<double>(numd);

	for (unsigned char i = 0; i < numd; ++i)
	{
		for (unsigned char j = 0; j < numd; ++j)
		{
			setDipole(Eigen::Vector2d(posx + static_cast<double>(i) * dl, posy + static_cast<double>(j) * dl));
		}
	}

	solve(_lenx, _leny, _numx, _numy);
}

SquareDDA::SquareDDA(double _lenx, double _leny, size_t _numx, size_t _numy, double _kvex, double _kvey, double _famp)
{
	setPlaneWave(Eigen::Vector2d(_kvex, _kvey), _famp);
	setDipole();

	lend = std::min(_lenx / 50.0, _leny / 50.0);

	posx = (_lenx - lend) / 2.0;
	posy = (_leny - lend) / 2.0;

	double dl = lend / static_cast<double>(numd);

	for (unsigned char i = 0; i < numd; ++i)
	{
		for (unsigned char j = 0; j < numd; ++j)
		{
			setDipole(Eigen::Vector2d(posx + static_cast<double>(i) * dl, posy + static_cast<double>(j) * dl));
		}
	}

	solve(_lenx, _leny, _numx, _numy);
}

SquareDDA::SquareDDA(double _lenx, double _leny, size_t _numx, size_t _numy, unsigned char _numd, double _lend, double _posx, double _posy)
{
	setDipole();

	numd = _numd;
	lend = _lend;

	posx = _posx;
	posy = _posy;

	double dl = lend / static_cast<double>(numd);

	for (unsigned char i = 0; i < numd; ++i)
	{
		for (unsigned char j = 0; j < numd; ++j)
		{
			setDipole(Eigen::Vector2d(posx + static_cast<double>(i) * dl, posy + static_cast<double>(j) * dl));
		}
	}

	solve(_lenx, _leny, _numx, _numy);
}

SquareDDA::SquareDDA(double _lenx, double _leny, size_t _numx, size_t _numy, unsigned char _numd, double _lend, double _posx, double _posy, double _kvex, double _kvey, double _famp)
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
		for (unsigned char j = 0; j < numd; ++j)
		{
			setDipole(Eigen::Vector2d(posx + static_cast<double>(i) * dl, posy + static_cast<double>(j) * dl));
		}
	}

	solve(_lenx, _leny, _numx, _numy);
}