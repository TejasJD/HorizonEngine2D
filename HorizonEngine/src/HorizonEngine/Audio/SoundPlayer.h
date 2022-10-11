#pragma once

#ifndef HZN_SOUNDSOURCE_H
#define HZN_SOUNDSOURCE_H

#include "HorizonEngine/Core/Core.h"
#include "pch.h"
#include "Logging/Logging.h"
#include <AL/al.h>

namespace Hzn
{
	class HZN_API SoundPlayer
	{
	public:

		SoundPlayer();

		bool Play(uint32_t buffer);
		bool Stop();
		bool Pause();
		bool isPlaying();

	private:
		
		float p_Pitch = 1.f;
		float p_Gain = 1.f;
		//the current location of the source
		float p_Position[3] = { 0,0,0 };
		//current velocity (speed and direction) of the source
		float p_Velocity[3] = { 0,0,0 };
		//Specifies whether source is looping
		bool p_LoopSound = false;
		ALuint p_Buffer = 0;
		ALint playState;
		ALuint p_Source;
	};
}

#endif // !HZN_SOUNDSOURCE_H

