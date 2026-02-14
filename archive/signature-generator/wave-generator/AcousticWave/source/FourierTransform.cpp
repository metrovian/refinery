#include "FourierTransform.h"
#include "Predefined.h"

FourierFunction::FourierFunction(const WaveFunction& _wave)
{
	wdata = _wave.getWaveData();
	header = _wave.getWaveHeader();
	fft();
}

bool FourierFunction::fft()
{
	size_t wsize = wdata.size();
	size_t tsize = 2;
	size_t power = 1;

	if (wsize < 2) return false;

	while (tsize < wsize)
	{
		tsize <<= 1;
		++power;
	}

	FourierData convert;
	for (size_t i = 0; i < tsize; ++i)
	{
		if (i < wsize)
		{
			convert.push_back(std::complex<double>(wdata[i], 0));
		}

		else
		{
			convert.push_back(std::complex<double>(0, 0));
		}
	}

	for (size_t i = 0; i < tsize; ++i)
	{
		size_t rev = 0;
		for (size_t j = 0; j < power; ++j)
		{
			rev = (rev << 1) | ((i >> j) & 1);
		}
		if (i < rev)
		{
			std::swap(convert[i], convert[rev]);
		}
	}

	for (size_t s = 1; s <= power; ++s)
	{
		size_t m = static_cast<size_t>(1) << s;
		std::complex<double> wm = std::exp(std::complex<double>(0, -2.0 * PI / (double)m));

		for (size_t k = 0; k < tsize; k += m)
		{
			std::complex<double> w = 1;
			for (size_t j = 0; j < m / 2; ++j)
			{
				std::complex<double> t = w * convert[k + j + m / 2];
				std::complex<double> u = convert[k + j];
				convert[k + j] = u + t;
				convert[k + j + m / 2] = u - t;
				w *= wm;
			}
		}
	}

	fdata = convert;

	return true;
}

bool FourierFunction::ifft()
{
	size_t fsize = fdata.size();
	size_t tsize = 2;
	size_t power = 1;

	if (fsize < 2) return false;

	while (tsize < fsize)
	{
		tsize <<= 1;
		++power;
	}

	FourierData convert;
	for (size_t i = 0; i < tsize; ++i)
	{
		if (i < fsize)
		{
			convert.push_back(fdata[i]);
		}

		else
		{
			convert.push_back(std::complex<double>(0, 0));
		}
	}

	for (size_t i = 0; i < tsize; ++i)
	{
		size_t rev = 0;
		for (size_t j = 0; j < power; ++j)
		{
			rev = (rev << 1) | ((i >> j) & 1);
		}
		if (i < rev)
		{
			std::swap(convert[i], convert[rev]);
		}
	}

	for (size_t s = 1; s <= power; ++s)
	{
		size_t m = static_cast<size_t>(1) << s;
		std::complex<double> wm = std::exp(std::complex<double>(0, 2.0 * PI / (double)m));

		for (size_t k = 0; k < tsize; k += m)
		{
			std::complex<double> w = 1;
			for (size_t j = 0; j < m / 2; ++j)
			{
				std::complex<double> t = w * convert[k + j + m / 2];
				std::complex<double> u = convert[k + j];
				convert[k + j] = u + t;
				convert[k + j + m / 2] = u - t;
				w *= wm;
			}
		}
	}

	WaveData ret;
	for (const auto& pt : convert)
	{
		ret.push_back((size_t)(pt.real() / (double)tsize));
	}

	ret.resize(wdata.size());
	wdata = ret;
	setWaveHeader(header.SAMPLE_RATE, header.BIT_PER_SAMPLE);

	return true;
}

FourierData FourierFunction::getFFT() const
{
	size_t wsize = wdata.size();
	size_t tsize = 2;
	size_t power = 1;

	if (wsize < 2) return FourierData();

	while (tsize < wsize)
	{
		tsize <<= 1;
		++power;
	}

	FourierData convert;
	for (size_t i = 0; i < tsize; ++i)
	{
		if (i < wsize)
		{
			convert.push_back(std::complex<double>(wdata[i], 0));
		}

		else
		{
			convert.push_back(std::complex<double>(0, 0));
		}
	}

	for (size_t i = 0; i < tsize; ++i)
	{
		size_t rev = 0;
		for (size_t j = 0; j < power; ++j)
		{
			rev = (rev << 1) | ((i >> j) & 1);
		}
		if (i < rev)
		{
			std::swap(convert[i], convert[rev]);
		}
	}

	for (size_t s = 1; s <= power; ++s)
	{
		size_t m = static_cast<size_t>(1) << s;
		std::complex<double> wm = std::exp(std::complex<double>(0, -2.0 * PI / (double)m));

		for (size_t k = 0; k < tsize; k += m)
		{
			std::complex<double> w = 1;
			for (size_t j = 0; j < m / 2; ++j)
			{
				std::complex<double> t = w * convert[k + j + m / 2];
				std::complex<double> u = convert[k + j];
				convert[k + j] = u + t;
				convert[k + j + m / 2] = u - t;
				w *= wm;
			}
		}
	}

	return convert;
}

FourierData FourierFunction::getIFFT() const
{
	size_t fsize = fdata.size();
	size_t tsize = 2;
	size_t power = 1;

	if (fsize < 2) return FourierData();

	while (tsize < fsize)
	{
		tsize <<= 1;
		++power;
	}

	FourierData convert;
	for (size_t i = 0; i < tsize; ++i)
	{
		if (i < fsize)
		{
			convert.push_back(fdata[i]);
		}

		else
		{
			convert.push_back(std::complex<double>(0, 0));
		}
	}

	for (size_t i = 0; i < tsize; ++i)
	{
		size_t rev = 0;
		for (size_t j = 0; j < power; ++j)
		{
			rev = (rev << 1) | ((i >> j) & 1);
		}
		if (i < rev)
		{
			std::swap(convert[i], convert[rev]);
		}
	}

	for (size_t s = 1; s <= power; ++s)
	{
		size_t m = static_cast<size_t>(1) << s;
		std::complex<double> wm = std::exp(std::complex<double>(0, 2.0 * PI / (double)m));

		for (size_t k = 0; k < tsize; k += m)
		{
			std::complex<double> w = 1;
			for (size_t j = 0; j < m / 2; ++j)
			{
				std::complex<double> t = w * convert[k + j + m / 2];
				std::complex<double> u = convert[k + j];
				convert[k + j] = u + t;
				convert[k + j + m / 2] = u - t;
				w *= wm;
			}
		}
	}

	FourierData ret;
	for (const auto& pt : convert)
	{
		ret.push_back((pt / (double)tsize));
	}

	ret.resize(wdata.size());
	return ret;
}

double FourierFunction::getFrequency(size_t _fdx) const
{
	return (double)_fdx * (double)header.SAMPLE_RATE / (double)fdata.size();
}

bool FourierFunction::setLPF(double _freq, double _brate)
{
	if (_brate > 0) return false;

	for (size_t i = 0; i < fdata.size() / 2; i++)
	{
		if (getFrequency(i) > _freq)
		{
			fdata[i] *= pow(10.0, _brate);
			fdata[fdata.size() - i - 1] *= pow(10, _brate);
		}
	}

	bool cond1 = ifft();
	bool cond2 = fft();

	return cond1 && cond2;
}

bool FourierFunction::setHPF(double _freq, double _brate)
{
	if (_brate > 0) return false;

	for (size_t i = 0; i < fdata.size() / 2; i++)
	{
		if (getFrequency(i) < _freq)
		{
			fdata[i] *= pow(10.0, _brate);
			fdata[fdata.size() - i - 1] *= pow(10, _brate);
		}
	}

	bool cond1 = ifft();
	bool cond2 = fft();

	return cond1 && cond2;
}

bool FourierFunction::setBPF(double _freq1, double _freq2, double _brate)
{
	if (_brate > 0) return false;

	for (size_t i = 0; i < fdata.size() / 2; i++)
	{
		double freq = getFrequency(i);
		if ((_freq1 < freq) && (freq < _freq2))
		{
			fdata[i] *= pow(10.0, _brate);
			fdata[fdata.size() - i - 1] *= pow(10, _brate);
		}
	}

	bool cond1 = ifft();
	bool cond2 = fft();

	return cond1 && cond2;
}

bool FourierFunction::setInverseFrequencyLPF(double _freq, double _invp)
{
	if (_freq < 0 || _invp < 0) return false;

	for (size_t i = 0; i < fdata.size() / 2; i++)
	{
		if (getFrequency(i) > _freq)
		{
			fdata[i] *= pow(_freq / getFrequency(i), _invp);
			fdata[fdata.size() - i - 1] *= pow(_freq / getFrequency(i), _invp);
		}
	}

	bool cond1 = ifft();
	bool cond2 = fft();

	return cond1 && cond2;
}

bool FourierFunction::exportWaveSpectrum(const std::string& _fname) const
{
	std::ofstream file(_fname + ".csv");

	if (!file)
	{
		return false;
	}

	for (size_t i = 0; i < fdata.size() / 2; ++i)
	{
		file << (double)i * (double)header.SAMPLE_RATE / (double)fdata.size() << ", " << sqrt(std::norm(fdata[i])) << std::endl;
	}

	return true;
}

