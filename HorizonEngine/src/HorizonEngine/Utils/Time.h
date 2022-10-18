#pragma once

#ifndef HZN_TIME_H
#define HZN_TIME_H

#include "HorizonEngine/Components/Component.h"

namespace Hzn {
	class Time {
	public:
		static float deltaTime;
		static float fixedDeltaTime;
	private:
		static std::chrono::duration<float> deltaTimeDuration;
		static std::chrono::duration<float> fixedDeltaTimeDuration;
		static std::chrono::high_resolution_clock::time_point lastUpdateTime;
		static std::chrono::high_resolution_clock::time_point lastFixedUpdateTime;
	public:
		Time();
		~Time() {}
	private:
		void awake();
		void start();
		void update();
		void fixedUpdate();
	};
}
#endif