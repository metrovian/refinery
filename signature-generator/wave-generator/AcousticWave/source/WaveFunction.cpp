#include "WaveFunction.h"
#include "Predefined.h"

bool WaveFunction::operator!=(const WaveFunction& _rhs) const
{
	if (!isWaveHeader(header)) return true;
	if (!isWaveHeader(_rhs.header)) return true;
	if (header.DATA_SIZE != _rhs.header.DATA_SIZE) return true;
	if (header.SAMPLE_RATE != _rhs.header.SAMPLE_RATE) return true;
	if (header.BIT_PER_SAMPLE != _rhs.header.BIT_PER_SAMPLE) return true;

	return false;
}

bool WaveFunction::operator==(const WaveFunction& _rhs) const
{
	return !(operator!=(_rhs));
}

WaveFunction WaveFunction::operator+(const WaveFunction& _rhs) const
{
	assert(operator==(_rhs));

	WaveFunction ret;
	ret.header = _rhs.header;
	ret.wdata.resize(wdata.size());

	for (int i = 0; i < wdata.size(); ++i)
	{
		ret.wdata[i] = wdata[i] + _rhs.wdata[i];
	}

	return ret;
}

WaveFunction WaveFunction::operator-(const WaveFunction& _rhs) const
{
	assert(operator==(_rhs));

	WaveFunction ret;
	ret.header = _rhs.header;
	ret.wdata.resize(wdata.size());

	for (int i = 0; i < wdata.size(); ++i)
	{
		ret.wdata[i] = wdata[i] - _rhs.wdata[i];
	}

	return ret;
}

WaveFunction WaveFunction::operator*(const WaveFunction& _rhs) const
{
	assert(operator==(_rhs));

	WaveFunction ret;
	ret.header = _rhs.header;
	ret.wdata.resize(wdata.size());

	for (size_t i = 0; i < wdata.size(); ++i)
	{
		ret.wdata[i] = wdata[i] * _rhs.wdata[i];
	}

	return ret;
}

WaveFunction WaveFunction::operator&(const WaveFunction& _rhs) const
{
	assert(isWaveHeader(header));
	assert(isWaveHeader(_rhs.header));
	assert(header.SAMPLE_RATE == _rhs.header.SAMPLE_RATE);
	assert(header.BIT_PER_SAMPLE == _rhs.header.BIT_PER_SAMPLE);

	WaveFunction ret;
	ret.header = header;
	ret.wdata = wdata;

	for (size_t i = 0; i < _rhs.wdata.size(); ++i)
	{
		ret.wdata.push_back(_rhs.wdata[i]);
	}

	ret.header.DATA_SIZE = (unsigned int)ret.wdata.size() * 2;
	ret.header.CHUNK_SIZE = header.DATA_SIZE + 36;

	return ret;
}

bool WaveFunction::isWaveHeader(const WaveHeader& _header)
{
	if (std::string(_header.RIFF, 4) != "RIFF") return false;
	if (std::string(_header.WAVE, 4) != "WAVE") return false;
	if (std::string(_header.FMT, 4) != "fmt ") return false;
	if (std::string(_header.DATA_ID, 4) != "data") return false;
	if (_header.FMT_SIZE != 16) return false;
	if (_header.AUDIO_FORMAT != 1) return false;
	if (_header.NUM_OF_CHANNEL != 1) return false;

	return true;
}

bool WaveFunction::isWaveFile(const std::string& _fname)
{
	std::ifstream file(_fname, std::ios::binary);
	if (!file) return false;

	WaveHeader head;
	file.read(reinterpret_cast<char*>(&head), sizeof(WaveHeader));
	if (!file) return false;

	return isWaveHeader(head);
}

bool WaveFunction::setWaveHeader(unsigned int _srate, unsigned short _sbit)
{
	if (_sbit % 8 != 0) return false;

	memcpy(header.RIFF, "RIFF", 4);
	memcpy(header.WAVE, "WAVE", 4);
	memcpy(header.FMT, "fmt ", 4);
	memcpy(header.DATA_ID, "data", 4);

	header.FMT_SIZE = 16;
	header.AUDIO_FORMAT = 1;
	header.NUM_OF_CHANNEL = 1;

	header.SAMPLE_RATE = _srate;
	header.BIT_PER_SAMPLE = _sbit;

	header.BLOCK_ALIGN = header.BIT_PER_SAMPLE / 8;
	header.BYTE_RATE = header.BLOCK_ALIGN * header.SAMPLE_RATE;

	header.DATA_SIZE = (unsigned int)wdata.size() * 2;
	header.CHUNK_SIZE = header.DATA_SIZE + 36;

	return true;
}

bool WaveFunction::setWaveData(const WaveData& _data)
{
	wdata.clear();
	wdata = _data;

	return true;
}

size_t WaveFunction::calcWaveDataSize(double _dura, unsigned int _srate)
{
	return (size_t)(_dura * (double)_srate);
}

WaveFunction::WaveFunction()
{
	mtxptr = new std::mutex();
}

WaveFunction::~WaveFunction()
{
	delete mtxptr;
}

bool WaveFunction::setWaveFunction(const WaveData& _data, const WaveHeader& _header)
{
	return setWaveFunction(_data, _header.SAMPLE_RATE, _header.BIT_PER_SAMPLE);
}

bool WaveFunction::setWaveFunction(const WaveData& _data, unsigned int _srate, unsigned short _sbit)
{
	bool cond1 = setWaveData(_data);
	bool cond2 = setWaveHeader(_srate, _sbit);

	return cond1 && cond2;
}

bool WaveFunction::setWaveFunction(const WaveData& _data)
{
	return setWaveFunction(_data, header.SAMPLE_RATE, header.BIT_PER_SAMPLE);
}

bool WaveFunction::exportWave(const std::string& _fname) const
{
	std::ofstream file(_fname + ".wav", std::ios::binary);

	if (!file)
	{
		return false;
	}

	file.write(reinterpret_cast<const char*>(&header), sizeof(header));
	file.write(reinterpret_cast<const char*>(wdata.data()), wdata.size() * 2);

	return true;
}

bool WaveFunction::importWave(const std::string& _fname)
{
	if (isWaveFile(_fname + ".wav"))
	{
		std::ifstream file(_fname + ".wav", std::ios::binary);

		header = WaveHeader();
		file.read(reinterpret_cast<char*>(&header), sizeof(WaveHeader));

		wdata.clear();
		wdata.resize(header.DATA_SIZE / 2);
		file.read(reinterpret_cast<char*>(wdata.data()), header.DATA_SIZE);

		if (file) return true;
	}

	header = WaveHeader();
	wdata.clear();

	return false;
}

void WaveFunction::playWave(double* _pch, double _namp)
{
	WAVEFORMATEX wfx;

	wfx.nSamplesPerSec = header.SAMPLE_RATE;
	wfx.wBitsPerSample = header.BIT_PER_SAMPLE;
	wfx.nChannels = header.NUM_OF_CHANNEL;
	wfx.cbSize = 0;
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nBlockAlign = (wfx.wBitsPerSample * wfx.nChannels) / 8;
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

	mtxptr->lock();

	handles.push(HWAVEOUT());
	headers.push(WAVEHDR());

	HWAVEOUT* handle = &(handles.back());
	WAVEHDR* whdr = &(headers.back());

	mtxptr->unlock();

	if (waveOutOpen(handle, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL) != MMSYSERR_NOERROR)
	{
		return;
	}

	std::vector<short> vdata = wdata;
	for (size_t i = 0; i < wdata.size(); ++i)
	{
		vdata[i] *= _namp;
	}

	whdr->lpData = (LPSTR)vdata.data();
	whdr->dwBufferLength = wdata.size() * 2;
	whdr->dwFlags = 0;
	whdr->dwLoops = 0;

	if (waveOutPrepareHeader(*handle, whdr, sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
	{
		mtxptr->lock();

		waveOutClose(*handle);

		handles.pop();
		headers.pop();

		mtxptr->unlock();

		return;
	}

	if (waveOutWrite(*handle, whdr, sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
	{
		mtxptr->lock();

		waveOutUnprepareHeader(*handle, whdr, sizeof(WAVEHDR));
		waveOutClose(*handle);

		handles.pop();
		headers.pop();

		mtxptr->unlock();

		return;
	}

	while (!(whdr->dwFlags & WHDR_DONE))
	{
		mtxptr->lock();

		if (!handles.empty())
		{
			waveOutSetPlaybackRate(*handle, static_cast<DWORD>((*_pch) * 65536.0));
		}

		mtxptr->unlock();

		Sleep(10);
	}
}

void WaveFunction::playWave()
{
	WAVEFORMATEX wfx;

	wfx.nSamplesPerSec = header.SAMPLE_RATE;
	wfx.wBitsPerSample = header.BIT_PER_SAMPLE;
	wfx.nChannels = header.NUM_OF_CHANNEL;
	wfx.cbSize = 0;
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nBlockAlign = (wfx.wBitsPerSample * wfx.nChannels) / 8;
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

	mtxptr->lock();

	handles.push(HWAVEOUT());
	headers.push(WAVEHDR());

	HWAVEOUT* handle = &(handles.back());
	WAVEHDR* whdr = &(headers.back());

	mtxptr->unlock();

	if (waveOutOpen(handle, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL) != MMSYSERR_NOERROR)
	{
		return;
	}

	whdr->lpData = (LPSTR)wdata.data();
	whdr->dwBufferLength = wdata.size() * 2;
	whdr->dwFlags = 0;
	whdr->dwLoops = 0;

	if (waveOutPrepareHeader(*handle, whdr, sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
	{
		mtxptr->lock();

		waveOutClose(*handle);

		handles.pop();
		headers.pop();

		mtxptr->unlock();

		return;
	}

	if (waveOutWrite(*handle, whdr, sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
	{
		mtxptr->lock();

		waveOutUnprepareHeader(*handle, whdr, sizeof(WAVEHDR));
		waveOutClose(*handle);

		handles.pop();
		headers.pop();

		mtxptr->unlock();

		return;
	}

	while (!(whdr->dwFlags & WHDR_DONE))
	{
		Sleep(100);
	}
}

void WaveFunction::stopWave()
{
	mtxptr->lock();

	if (!handles.empty())
	{
		waveOutReset(handles.front());

		waveOutUnprepareHeader(handles.front(), &headers.front(), sizeof(WAVEHDR));
		waveOutClose(handles.front());

		handles.pop();
		headers.pop();
	}

	mtxptr->unlock();
}

void WaveFunction::stopWave(bool* _sustain)
{
	while (*_sustain)
	{
		Sleep(100);
	}

	stopWave();
}

WaveHeader WaveFunction::getWaveHeader() const
{
	return header;
}

WaveData WaveFunction::getWaveData() const
{
	return wdata;
}

short WaveFunction::getWaveData(size_t _tdx) const
{
	assert(_tdx < wdata.size());
	return wdata[_tdx];
}

WaveFunction WaveFunction::sin(double _namp, double _freq, double _dura, unsigned int _srate, unsigned short _sbit)
{
	WaveFunction ret;
	WaveData dat;

	assert(_namp < 1.0);
	double ramp = _namp * pow(2.0, (double)_sbit - 1.0);
	double unit = 1.0 / (double)_srate;
	size_t size = calcWaveDataSize(_dura, _srate);

	double tomg = 2.0 * PI * _freq;

	for (double i = 0; i < size; ++i)
	{
		double sample = ramp * std::sin(tomg * i * unit);
		dat.push_back((short)sample);
	}

	ret.setWaveFunction(dat, _srate, _sbit);

	return ret;
}

WaveFunction WaveFunction::sqr(double _namp, double _freq, double _dura, unsigned int _srate, unsigned short _sbit, double _duty)
{
	WaveFunction ret;
	WaveData dat;

	assert(_namp < 1.0);
	double ramp = _namp * pow(2.0, (double)_sbit - 1.0);
	double unit = 1.0 / (double)_srate;
	double period = 1.0 / _freq;
	size_t size = calcWaveDataSize(_dura, _srate);

	auto step = [period, ramp, _duty](double time)
		{
			double rem = time / period;
			rem -= floor(rem);

			if (rem < _duty) return ramp;
			return -ramp;
		};

	for (double i = 0; i < size; ++i)
	{
		dat.push_back((short)step(unit * i));
	}

	ret.setWaveFunction(dat, _srate, _sbit);

	return ret;
}

WaveFunction WaveFunction::tri(double _namp, double _freq, double _dura, unsigned int _srate, unsigned short _sbit)
{
	WaveFunction ret;
	WaveData dat;

	assert(_namp < 1.0);
	double ramp = _namp * pow(2.0, (double)_sbit - 1.0);
	double unit = 1.0 / (double)_srate;
	double period = 1.0 / _freq;
	size_t size = calcWaveDataSize(_dura, _srate);

	auto step = [period, ramp](double time)
		{
			double rem = time / period;
			rem -= floor(rem);
			rem *= 4.0;

			if (rem > 1.0) return ramp * rem;
			else if (rem > 3.0) return ramp * (2.0 - rem);
			else return ramp * (rem - 4.0);
		};

	for (double i = 0; i < size; ++i)
	{
		dat.push_back((short)step(unit * i));
	}

	ret.setWaveFunction(dat, _srate, _sbit);

	return ret;
}

WaveFunction WaveFunction::saw(double _namp, double _freq, double _dura, unsigned int _srate, unsigned short _sbit, bool _reverse)
{
	WaveFunction ret;
	WaveData dat;

	assert(_namp < 1.0);
	double ramp = _namp * pow(2.0, (double)_sbit - 1.0);
	double unit = 1.0 / (double)_srate;
	double period = 1.0 / _freq;
	size_t size = calcWaveDataSize(_dura, _srate);

	auto step = [period, ramp, _reverse](double time)
		{
			double rem = time / period;
			rem -= floor(rem);
			rem *= 2.0;

			if (_reverse) return ramp * (1.0 - rem);
			return ramp * (rem - 1.0);
		};

	for (double i = 0; i < size; ++i)
	{
		dat.push_back((short)step(unit * i));
	}

	ret.setWaveFunction(dat, _srate, _sbit);

	return ret;
}

WaveFunction WaveFunction::ofs(double _ofs, double _dura, unsigned int _srate, unsigned short _sbit)
{
	WaveFunction ret;
	WaveData dat;

	size_t size = calcWaveDataSize(_dura, _srate);

	for (double i = 0; i < size; ++i)
	{
		dat.push_back((short)_ofs);
	}

	ret.setWaveFunction(dat, _srate, _sbit);

	return ret;
}
