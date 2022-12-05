#pragma once

#ifndef HZN_TIME_H
#define HZN_TIME_H

namespace Hzn {
	class Time {
	public:
		//!Time variable for store time elapsed since last frame update
		static float deltaTime;
		//!Time variable for store time elapsed since last fixed frame update
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