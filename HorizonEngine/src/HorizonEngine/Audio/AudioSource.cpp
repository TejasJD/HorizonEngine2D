#include "AudioSource.h"

namespace Hzn
{

	AudioSource::AudioSource()
	{
		alGenSources(1, &p_Source);
		alSourcef(p_Source, AL_PITCH, p_Pitch);
		alSourcef(p_Source, AL_GAIN, p_Gain);
		alSource3f(p_Source, AL_POSITION, p_Position[0], p_Position[1], p_Position[2]);
		alSource3f(p_Source, AL_VELOCITY, p_Velocity[0], p_Velocity[1], p_Velocity[2]);
		alSourcei(p_Source, AL_LOOPING, p_LoopSound);
		alSourcei(p_Source, AL_BUFFER, p_Buffer);
	}
	AudioSource::~AudioSource()
	{
	}
	bool AudioSource::Play()
	{

		alSourcei(p_Source, AL_BUFFER, (ALint)p_Buffer);
		alSourcePlay(p_Source);

		HZN_CORE_INFO("playing sound :  {0}", p_Source);

		if (alGetError() == AL_NO_ERROR)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	bool AudioSource::Stop()
	{
		HZN_CORE_INFO("stop sound :  {0}", p_Source);

		alSourceStop(p_Source);

		if (alGetError() == AL_NO_ERROR)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	bool AudioSource::Pause()
	{
		HZN_CORE_INFO("pause sound :  {0}", p_Source);

		alSourcePause(p_Source);

		if (alGetError() == AL_NO_ERROR)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	bool AudioSource::isPlaying()
	{
		alGetSourcei(p_Source, AL_SOURCE_STATE, &playState);
		return (playState == AL_PLAYING);
	}
	void AudioSource::init(const char* filename)
	{
		p_Buffer = Hzn::SoundBuffer::Init()->LoadSound(filename);
	}
}