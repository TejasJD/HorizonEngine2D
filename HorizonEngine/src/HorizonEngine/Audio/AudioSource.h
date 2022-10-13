#pragma once

#ifndef HZN_AUDIOSOURCE_H
#define HZN_AUDIOSOURCE_H

#include "HorizonEngine/Core/Core.h"
#include "HorizonEngine/Components/Component.h"
#include <Audio/SoundBuffer.h>

namespace Hzn
{
	class HZN_API AudioSource : public Component
	{
	public:
		AudioSource();
		~AudioSource();
		void init(const char* filename);
		virtual void awake() override{};
		virtual void start() override{};
		virtual void update() override{};
		virtual void fixedUpdate() override{};
		bool Play();
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


#endif