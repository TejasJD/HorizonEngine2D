#pragma once

#ifndef _math_h
#define _math_h

#include <limits>
#include <stdlib.h>
#include <cmath>

class Math {
public:
	static float PI = 3.14159265358979f;
	static float deg2rad = (PI * 2.0f) / 360.0f;
	static float rad2deg = 360.0f / (PI * 2.0f);
	static float infinity = FLT_MAX;
	static float negativeInfinity = -FLT_MAX;
public:
	static float abs(float f);
	static float acos(float f);
	static float asin(float f);
	static float atan(float f);
	static float atan2(float x, float y);
	static float ceil(float f);
	static int ceilToInt(float f);
	static float clamp(float f, float min, float max);
	static float clamp01(float f);
	static float cos(float f);
	static float deltaAngle(float current, float target);
	static float exp(float power);
	static float floor(float f);
	static int floorToInt(float f);
	static float inverseLerp(float start, float end, float f);
	static float lerp(float start, float end, float f);
	static float lerpAngle(float start, float end, float f);
	static float lerpUnclamped(float start, float end, float f);
	static float max(float a, float b);
	static float min(float a, float b);
	static float moveTowards(float start, float end, float maxDelta);
	static float moveTowardsAngle(float start, float end, float maxDelta);
	static float pingPong(float value, float max);
	static float pow(float f, float power);
	static float round(float f);
	static int roundToInt(float f);
	static int sign(float f);
	static float sin(float f);
	static float sqrt(float f);
	static float tan(float f);
};

#endif // !_math_h
