#pragma once
#ifndef HZN_SOUNDBUFFER_H
#define HZN_SOUNDBUFFER_H
#include "HorizonEngine/Core/Core.h"

#include <AL/al.h>
#include "pch.h"

#include <sndfile.h>

#include <AL/alext.h>
#include <Logging/Logging.h>

namespace Hzn
{
	class HZN_API SoundBuffer
	{
	public:
		static SoundBuffer* Init();

		ALuint LoadSound(const char* filename);

	private:
		SoundBuffer();
		~SoundBuffer();

		std::vector<ALuint> p_SoundBuffers;
	};

}
#endif
