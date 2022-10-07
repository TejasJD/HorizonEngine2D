#pragma once

#ifndef HZN_SOUNDSOURCE_H
#define HZN_SOUNDSOURCE_H

#include "HorizonEngine/Core/Core.h"
#include <AL/al.h>

namespace Hzn
{
	class HZN_API SoundSource
	{
	public:
		SoundSource();
		~SoundSource();

		bool Play(std::vector<uint32_t> buffers);

	private:
		ALuint p_Source_1;
		ALuint p_Source_2;
		ALuint p_Source_3;
		float p_Pitch = 1.f;
		float p_Gain = 1.f;
		float p_Position[3] = { 0,0,0 };
		float p_Velocity[3] = { 0,0,0 };
		bool p_LoopSound = false;
		ALuint p_Buffer = 0;
	};
}

#endif // !HZN_SOUNDSOURCE_H

