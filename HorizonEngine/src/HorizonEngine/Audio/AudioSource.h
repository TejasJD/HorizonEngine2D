#pragma once

#ifndef HZN_AUDIOSOURCE_H
#define HZN_AUDIOSOURCE_H

#include <cstdint>
#include <memory>

//#include "HorizonEngine/Core/Core.h"
//#include "HorizonEngine/Components/Component.h"
#include "HorizonEngine/Audio/SoundBuffer.h"

namespace Hzn
{
	/// <summary>
	/// 
	/// </summary>
	class AudioSource
	{
	public:
		
		AudioSource(uint32_t p_Buffer);
		~AudioSource();
		static std::shared_ptr<AudioSource> init(const char* path);

		bool Play();
		bool Stop();
		bool Pause();
		bool Resume();
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
		uint32_t p_Buffer;
		int32_t playState;
		uint32_t p_Source;
	};
}


#endif