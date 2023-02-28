#pragma once

#ifndef HZN_SOUNDBUFFER_H
#define HZN_SOUNDBUFFER_H

#include <cstdint>
#include <vector>

#include "HorizonEngine/Core/Core.h"

namespace Hzn
{
	class HZN_API SoundBuffer
	{
	public:
		static SoundBuffer* Init();

		uint32_t LoadSound(const char* filename);

	private:
		SoundBuffer();
		~SoundBuffer();

		std::vector<uint32_t> p_SoundBuffers;
	};

}
#endif
