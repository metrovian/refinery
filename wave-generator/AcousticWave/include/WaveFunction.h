#pragma once
#include <Windows.h>
#include <vector>
#include <queue>
#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>
#include <mutex>

#pragma pack(push, 1)
struct WaveHeader
{
	char RIFF[4];                       // determined "RIFF"
	unsigned int CHUNK_SIZE;            // induced
	char WAVE[4];                       // determined "WAVE"
	char FMT[4];                        // determined "fmt "

	unsigned int FMT_SIZE;              // determined 16(PCM)
	unsigned short AUDIO_FORMAT;        // determined 1(PCM)
	unsigned short NUM_OF_CHANNEL;      // determined 1(MONO)
	unsigned int SAMPLE_RATE;           // user
	unsigned int BYTE_RATE;             // induced
	unsigned short BLOCK_ALIGN;         // induced
	unsigned short BIT_PER_SAMPLE;      // user
	char DATA_ID[4];                    // determined "data "
	unsigned int DATA_SIZE;             // induced
};
#pragma pack(pop)

typedef std::vector<short> WaveData;
class WaveFunction
{
protected: /* data */
	WaveHeader header;
	WaveData wdata;

private: /* handle */
	std::queue<HWAVEOUT> handles;
	std::queue<WAVEHDR> headers;

private: /* mutex */
	std::mutex* mtxptr;

public: /* operator */
	bool operator!=(const WaveFunction& _rhs) const;
	bool operator==(const WaveFunction& _rhs) const;

	WaveFunction operator+(const WaveFunction& _rhs) const;
	WaveFunction operator-(const WaveFunction& _rhs) const;
	WaveFunction operator*(const WaveFunction& _rhs) const;
	WaveFunction operator&(const WaveFunction& _rhs) const;

protected: /* parts */
	static bool isWaveHeader(const WaveHeader& _header);
	static bool isWaveFile(const std::string& _fname);

	bool setWaveHeader(unsigned int _srate, unsigned short _sbit);
	bool setWaveData(const WaveData& _data);

	static size_t calcWaveDataSize(double _dura, unsigned int _srate);

public: /* constructor */
	WaveFunction();
	~WaveFunction();

public: /* public use */
	bool setWaveFunction(const WaveData& _data, const WaveHeader& _header);
	bool setWaveFunction(const WaveData& _data, unsigned int _srate, unsigned short _sbit);
	bool setWaveFunction(const WaveData& _data);

	bool exportWave(const std::string& _fname) const;
	bool importWave(const std::string& _fname);

	void playWave(double* _pch, double _namp);
	void playWave();
	void stopWave();
	void stopWave(bool* _sustain);

	WaveHeader getWaveHeader() const;
	WaveData getWaveData() const;
	short getWaveData(size_t _tdx) const;

	static WaveFunction sin(double _namp, double _freq, double _dura, unsigned int _srate, unsigned short _sbit);
	static WaveFunction sqr(double _namp, double _freq, double _dura, unsigned int _srate, unsigned short _sbit, double _duty);
	static WaveFunction tri(double _namp, double _freq, double _dura, unsigned int _srate, unsigned short _sbit);
	static WaveFunction saw(double _namp, double _freq, double _dura, unsigned int _srate, unsigned short _sbit, bool _reverse);
	static WaveFunction ofs(double _ofs, double _dura, unsigned int _srate, unsigned short _sbit);
};