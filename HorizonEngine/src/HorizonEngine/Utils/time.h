#pragma once

#ifndef _time_h
#define _time_h

#include "../components/component.h"

#include <chrono>

namespace Hzn {
	class Time : public Component {
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
		~Time();
	private:
		void awake();
		void start();
		void update();
		void fixedUpdate();
	};
}
#endif