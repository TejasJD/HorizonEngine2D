#include "time.h"

using namespace std::chrono;

namespace Hzn {

	float Time::deltaTime;
	float Time::fixedDeltaTime;

	duration<float> Time::deltaTimeDuration;
	duration<float> Time::fixedDeltaTimeDuration;
	high_resolution_clock::time_point Time::lastUpdateTime;
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

	void Time::update() {
		high_resolution_clock::time_point currentUpdateTime = high_resolution_clock::now();
		deltaTimeDuration = currentUpdateTime - lastUpdateTime;
		deltaTime = deltaTimeDuration.count();
		lastUpdateTime = currentUpdateTime;
	}

	void Time::fixedUpdate() {
		high_resolution_clock::time_point currentFixedUpdateTime = high_resolution_clock::now();
		fixedDeltaTimeDuration = currentFixedUpdateTime - lastFixedUpdateTime;
		fixedDeltaTime = fixedDeltaTimeDuration.count();
		lastFixedUpdateTime = currentFixedUpdateTime;
	}
}
