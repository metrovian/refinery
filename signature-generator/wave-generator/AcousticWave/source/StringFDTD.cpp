#include "StringFDTD.h"
#include "Predefined.h"

StringFDTD::StringFDTD(double _length, double _period, size_t _numt, double _tension, double _density)
{
	setBasicCondition(sqrt(_tension / _density), _length, _period, _numt);
	solve();
}

StringFDTD::StringFDTD(double _length, double _period, size_t _numt, double _tension, double _density, double _decay)
{
	decay = _decay;

	setBasicCondition(sqrt(_tension / _density), _length, _period, _numt);
	solve();
}

bool StringFDTD::solve()
{
	wave[0] = generateImpulseCondition(0.7, 0.8);
	wave[1] = wave[0];

	setFixedEndCondition(wave[0]);
	setFixedEndCondition(wave[1]);

	for (size_t n = 1; n < numt - 1; ++n)
	{
		for (size_t i = 1; i < numx - 1; ++i)
		{
			wave[n + 1][i] = 0;
			wave[n + 1][i] += wave[n][i] * (2.0 + decay * dt);
			wave[n + 1][i] -= wave[n - 1][i];
			wave[n + 1][i] += courant * courant * (wave[n][i + 1] - 2.0 * wave[n][i] + wave[n][i - 1]);
			wave[n + 1][i] /= 1.0 + decay * dt;
		}

		setFixedEndCondition(wave[n + 1]);
	}

	return true;
}
