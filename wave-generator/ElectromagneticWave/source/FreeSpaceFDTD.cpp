#include "FreeSpaceFDTD.h"

FreeSpaceFDTD::FreeSpaceFDTD(MODE _mode, double _lenx, double _leny, double _period, size_t _numx, size_t _numy)
{
	posx = _lenx / 2.0;
	posy = _leny / 2.0;

	solve(_mode, _lenx, _leny, _period, _numx, _numy);
}

FreeSpaceFDTD::FreeSpaceFDTD(MODE _mode, double _lenx, double _leny, double _period, size_t _numx, size_t _numy, double _posx, double _posy, double _famp, double _freq)
{
	posx = _posx;
	posy = _posy;

	famp = _famp;
	freq = _freq;

	solve(_mode, _lenx, _leny, _period, _numx, _numy);
}

FreeSpaceFDTD::FreeSpaceFDTD(MODE _mode, double _lenx, double _leny, double _period, size_t _numx, size_t _numy, size_t _numt)
{
	posx = _lenx / 2.0;
	posy = _leny / 2.0;

	solve(_mode, _lenx, _leny, _period, _numx, _numy, _numt);
}

FreeSpaceFDTD::FreeSpaceFDTD(MODE _mode, double _lenx, double _leny, double _period, size_t _numx, size_t _numy, size_t _numt, double _posx, double _posy, double _famp, double _freq)
{
	posx = _posx;
	posy = _posy;

	famp = _famp;
	freq = _freq;

	solve(_mode, _lenx, _leny, _period, _numx, _numy, _numt);
}

bool FreeSpaceFDTD::solve(MODE _mode, double _lenx, double _leny, double _period, size_t _numx, size_t _numy)
{
	WaveField init = generateHuygensSource(_mode, _lenx, _leny, _numx, _numy, posx, posy, 0.0, famp, freq);

	if (!setBasicCondition(init, _period)) return false;

	for (size_t i = 1; i < numt; ++i)
	{
		WaveField refresh = calcNextStepField(wave[i - 1]);
		WaveField source = generateHuygensSource(_mode, _lenx, _leny, _numx, _numy, _lenx / 2.0, _leny / 2.0, calcElapsedTime(), famp, freq);

		wave.push_back(refresh + source);
	}

	return true;
}

bool FreeSpaceFDTD::solve(MODE _mode, double _lenx, double _leny, double _period, size_t _numx, size_t _numy, size_t _numt)
{
	WaveField init = generateHuygensSource(_mode, _lenx, _leny, _numx, _numy, posx, posy, 0.0, famp, freq);

	if (!setBasicCondition(init, _period, _numt)) return false;

	for (size_t i = 1; i < numt; ++i)
	{
		WaveField refresh = calcNextStepField(wave[i - 1]);
		WaveField source = generateHuygensSource(_mode, _lenx, _leny, _numx, _numy, _lenx / 2.0, _leny / 2.0, calcElapsedTime(), famp, freq);

		wave.push_back(refresh + source);
	}

	return true;
}
