#pragma once
#include "EigenMatrix.h"
#include "WaveFunction.h"
#include "FourierTransform.h"

typedef std::queue<short> DelayData;
class DigitalWaveguide : public WaveFunction
{
protected: /* parts */
	WaveData extractFrontData(const DelayData& _data) const;
	WaveData extractFrontData(const DelayData& _data, size_t _count) const;

protected: /* delay */
	DelayData calcRandomDelayLine(double _namp, double _freq);
	DelayData calcBandLimDelayLine(double _namp, double _freq, unsigned char _band);
	DelayData calcImpulseDelayLine(double _namp, double _freq, double _istar, double _iend);

protected: /* filter */
	short passSimpleLPF(const DelayData& _data, unsigned char _pow) const;
	short passDynamicLPF(const DelayData& _data, short _prev, double _freq) const;
	short passStringAPF(const DelayData& _data, short _prev, double _damp) const;
	short passStringDF(const DelayData& _data, double _freq, double _damp, double _decay) const;

protected: /* batch-filter */
	DelayData passStringElasticModulusLPF(const DelayData& _data, double _mod) const;
	DelayData passStringPositionCF(const DelayData& _data, double _pos) const;
	DelayData passSoundBoardCMF(const DelayData& _data, double _freq, double _invp, unsigned int _srate, unsigned short _sbit) const;
	WaveData passAutoRegressionLPC(const WaveData& _data, unsigned char _num) const;

public: /* public use */
	WaveFunction castWaveFunction();

protected: /* virtual */
	virtual bool synthesis(double _namp, double _freq, double _dura, unsigned int _srate, unsigned short _sbit) = 0;
};