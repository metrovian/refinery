#include "MIDI.h"
#include "Predefined.h"

void MIDI::inputCallback(HMIDIIN hMidiIn, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
	static BYTE KEY_SUSTAIN = 64;

	if (wMsg == MIM_DATA)
	{
		DWORD msg = dwParam1;
		BYTE status = msg & 0xFF;
		BYTE key = (msg >> 8) & 0xFF;
		BYTE vel = (msg >> 16) & 0xFF;

		MIDI* device = reinterpret_cast<MIDI*>(dwInstance);

		switch (status)
		{

		case MIDI_KEY_DOWN:
		{
			std::thread trd_start = std::thread(static_cast<void(WaveFunction::*)(double*, double)>(&WaveFunction::playWave), &device->sound[key - 21], &device->pchs, device->vamps[vel]);
			trd_start.detach();

			break;
		}

		case MIDI_KEY_REL:
		{
			std::thread trd_stop = std::thread(static_cast<void(WaveFunction::*)(bool*)>(&WaveFunction::stopWave), &device->sound[key - 21], &device->sustain);
			trd_stop.detach();

			break;
		}

		case MIDI_SUSTAIN:
		{
			if (key == KEY_SUSTAIN)
			{
				if (vel > 0)
				{
					device->sustain = true;
				}

				else
				{
					device->sustain = false;
				}
			}

			break;
		}

		case MIDI_PITCH:
		{
			device->setPchValue(vel);
			break;
		}

		default:
		{
			std::cout
				<< "MIDI Message Received - Status: " << (int)status
				<< ", Data1: " << (int)key
				<< ", Data2: " << (int)vel << std::endl;

			break;
		}

		}
	}
}

void MIDI::setPchValue(char _pch)
{
	pchs = pow(2.0, ((double)_pch - 64) * pmax / 32.0 / 12.0);
}

bool MIDI::open(unsigned int _id)
{
	if (midiInOpen(&midi, _id, (DWORD_PTR)inputCallback, (DWORD_PTR)this, CALLBACK_FUNCTION) != MMSYSERR_NOERROR) return false;
	return true;
}

bool MIDI::start()
{
	if (midiInStart(midi) != MMSYSERR_NOERROR)
	{
		midiInClose(midi);
		return false;
	}

	return true;
}

bool MIDI::stop()
{
	if (midiInStop(midi) != MMSYSERR_NOERROR) return false;
	return true;
}

bool MIDI::close()
{
	if (midiInClose(midi) != MMSYSERR_NOERROR) return false;
	return true;
}

bool MIDI::setSound(WaveFunction(*_wave)(double _freq, double _dura), double _fdura)
{
	for (int i = 0; i < 88; i++)
	{
		sound[i] = _wave(A0 * pow(2.0, (double)i / 12.0), _fdura);
		std::cout << i << std::endl;
	}

	return true;
}

bool MIDI::setPchMaximum(double _pmax)
{
	pmax = _pmax;
	return true;
}

bool MIDI::setVampFunction(std::vector<double> _vamps)
{
	if (_vamps.size() != 127) return false;

	vamps = _vamps;
	return true;
}

std::vector<double> MIDI::vampConstant()
{
	std::vector<double> ret;

	for (char i = 0; i < 127; ++i)
	{
		ret.push_back(1.0);
	}

	return ret;
}

std::vector<double> MIDI::vampLinear()
{
	std::vector<double> ret;

	for (char i = 0; i < 127; ++i)
	{
		ret.push_back((double)i / 127.0);
	}

	return ret;
}