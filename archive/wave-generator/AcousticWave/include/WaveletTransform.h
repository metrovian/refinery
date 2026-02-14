#pragma once
#include "FourierTransform.h"

typedef std::vector<FourierData> WaveletData;
class MorletFunction : public FourierFunction
{
private: /* data */
	WaveletData tdata;
	std::vector<double> freqs;

private: /* parts */
	double getFourierDomainMorlet(double _freq, double _treso, size_t _idx);
	FourierData getMorletTransform(double _freq, double _treso);

public: /* constructor */
	MorletFunction(const FourierFunction& _wave, double _freq, double _treso);
	MorletFunction(const FourierFunction& _wave, double _freq1, double _freq2, double _treso);
	MorletFunction(const FourierFunction& _wave, double _freq1, double _freq2, double _freso, double _treso);

public: /* public use */
	bool exportWaveletSpectrum(std::string _fname);
};