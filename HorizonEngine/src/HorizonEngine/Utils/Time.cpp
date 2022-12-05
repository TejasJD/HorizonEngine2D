#include "pch.h"
#include "Time.h"

using namespace std::chrono;

namespace Hzn {

	//! get deltaTime from Time class
	float Time::deltaTime;
	//! get fixedDeltaTime from Time class
	float Time::fixedDeltaTime;

	//! Represent a time interval for Delta Time
	duration<float> Time::deltaTimeDuration;
	//! Represent a time interval for fixed Delta Time
	duration<float> Time::fixedDeltaTimeDuration;

	//! Represent last time update with the smallest tick period
	high_resolution_clock::time_point Time::lastUpdateTime;
	//! Represent last fixed time update with the smallest tick period
	high_resolution_clock::time_point Time::lastFixedUpdateTime;

	Time::Time() {
		// TODO: Attach awake, start update and fixedUpdate 
		// to main game loop
	}

	void Time::awake() {
		// May do this in the constructor instead?
		lastUpdateTime = high_resolution_clock::now();
		lastFixedUpdateTime = lastUpdateTime;
	}

	void Time::start() { }

	//! Tells the amount of time since the last frame update Value can be inconsistent and can fluctuate
	void Time::update() {
		high_resolution_clock::time_point currentUpdateTime = high_resolution_clock::now();
		deltaTimeDuration = currentUpdateTime - lastUpdateTime;
		deltaTime = deltaTimeDuration.count();
		lastUpdateTime = currentUpdateTime;
	}

	//!Tells the time passed since the last fixed frame updates
	void Time::fixedUpdate() {
		high_resolution_clock::time_point currentFixedUpdateTime = high_resolution_clock::now();
		fixedDeltaTimeDuration = currentFixedUpdateTime - lastFixedUpdateTime;
		fixedDeltaTime = fixedDeltaTimeDuration.count();
		lastFixedUpdateTime = currentFixedUpdateTime;
	}
}
