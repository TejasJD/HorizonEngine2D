#pragma once
#ifndef HZN_SOUNDBUFFER_H
#define HZN_SOUNDBUFFER_H
#include "HorizonEngine/Core/Core.h"

#include <AL/al.h>
#include <vector>
namespace Hzn
{
	class HZN_API SoundBuffer
	{
	public:
		static SoundBuffer* get();

		ALuint addSound(const char* filename);
		bool removeSound(const ALuint& buffer);

	private:
		SoundBuffer();
		~SoundBuffer();

		std::vector<ALuint> p_SoundBuffers;
	};

}
#endif
