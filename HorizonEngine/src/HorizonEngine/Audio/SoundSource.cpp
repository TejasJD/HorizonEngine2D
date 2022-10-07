#include "SoundSource.h"
#include <iostream>

namespace Hzn
{
	SoundSource::SoundSource()
	{

		
			alGenSources(1, &(p_Source_1));
			alSourcef(p_Source_1, AL_PITCH, p_Pitch);
			alSourcef(p_Source_1, AL_GAIN, p_Gain);
			alSource3f(p_Source_1, AL_POSITION, p_Position[0], p_Position[1], p_Position[2]);
			alSource3f(p_Source_1, AL_VELOCITY, p_Velocity[0], p_Velocity[1], p_Velocity[2]);
			alSourcei(p_Source_1, AL_LOOPING, p_LoopSound);
			alSourcei(p_Source_1, AL_BUFFER, p_Buffer);

			alGenSources(1, &(p_Source_2));
			alSourcef(p_Source_2, AL_PITCH, p_Pitch);
			alSourcef(p_Source_2, AL_GAIN, p_Gain);
			alSource3f(p_Source_2, AL_POSITION, p_Position[0], p_Position[1], p_Position[2]);
			alSource3f(p_Source_2, AL_VELOCITY, p_Velocity[0], p_Velocity[1], p_Velocity[2]);
			alSourcei(p_Source_2, AL_LOOPING, p_LoopSound);
			alSourcei(p_Source_2, AL_BUFFER, p_Buffer);

			alGenSources(1, &(p_Source_3));
			alSourcef(p_Source_3, AL_PITCH, p_Pitch);
			alSourcef(p_Source_3, AL_GAIN, p_Gain);
			alSource3f(p_Source_3, AL_POSITION, p_Position[0], p_Position[1], p_Position[2]);
			alSource3f(p_Source_3, AL_VELOCITY, p_Velocity[0], p_Velocity[1], p_Velocity[2]);
			alSourcei(p_Source_3, AL_LOOPING, p_LoopSound);
			alSourcei(p_Source_3, AL_BUFFER, p_Buffer);
		
	}

	SoundSource::~SoundSource()
	{
		
	}

	bool SoundSource::Play(std::vector<uint32_t> buffers)
	{

		if (buffers.size() == 0 || buffers.size() > 3) {
			return false;
		}

		ALint state = AL_PLAYING;
		
		switch (buffers.size()) {
		case 1:
			alSourcei(p_Source_1, AL_BUFFER, (ALint)buffers[0]);
			alSourcePlay(p_Source_1);
			
			while (state == AL_PLAYING && alGetError() == AL_NO_ERROR)
			{
				alGetSourcei(p_Source_1, AL_SOURCE_STATE, &state);
			}
			break;
		case 2:
			alSourcei(p_Source_1, AL_BUFFER, (ALint)buffers[0]);
			alSourcei(p_Source_2, AL_BUFFER, (ALint)buffers[1]);
			alSourcePlay(p_Source_1);
			alSourcePlay(p_Source_2);
			
			while (state == AL_PLAYING && alGetError() == AL_NO_ERROR)
			{
				alGetSourcei(p_Source_1, AL_SOURCE_STATE, &state);
				alGetSourcei(p_Source_2, AL_SOURCE_STATE, &state);
			}
			break;
		case 3:
			alSourcei(p_Source_1, AL_BUFFER, (ALint)buffers[0]);
			alSourcei(p_Source_2, AL_BUFFER, (ALint)buffers[1]);
			alSourcei(p_Source_3, AL_BUFFER, (ALint)buffers[2]);

			alSourcePlay(p_Source_1);
			alSourcePlay(p_Source_2);
			alSourcePlay(p_Source_3);

			while (state == AL_PLAYING && alGetError() == AL_NO_ERROR)
			{
				alGetSourcei(p_Source_1, AL_SOURCE_STATE, &state);
				alGetSourcei(p_Source_2, AL_SOURCE_STATE, &state);
				alGetSourcei(p_Source_3, AL_SOURCE_STATE, &state);
			}
			break;
		}

		if (alGetError() == AL_NO_ERROR)
		{
			return true;
		}
		else
		{
			return false;
		}
		
	}
}

