#include "pch.h" 
#include "math.h"

namespace Hzn {

	float Math::abs(float f) {
		return fabs(f);
	}

	float Math::acos(float f) {
		return acosf(f);
	}

	float Math::asin(float f) {
		return asinf(f);
	}

	float Math::atan(float f) {
		return atanf(f);
	}

	float Math::atan2(float x, float y) {
		return atan2f(x, y);
	}

	float Math::ceil(float f) {
		return ceilf(f);
	}

	int Math::ceilToInt(float f) {
		return (int)ceilf(f);
	}

	float Math::clamp(float f, float min, float max) {
		if (f < min) return min;
		if (f > max) return max;
		return f;
	}

	float Math::clamp01(float f) {
		return clamp(f, 0.0f, 1.0f);
	}

	float Math::cos(float f) {
		return cosf(f);
	}

	float Math::deltaAngle(float current, float target) {
		current = fmodf(current, 360.0f) + 360.0f;
		target = fmodf(target, 360.0f) + 360.0f;
		return abs(current - target);
	}

	float Math::exp(float power) {
		return expf(power);
	}

	float Math::floor(float f) {
		return floorf(f);
	}

	int Math::floorToInt(float f) {
		return (int)floorf(f);
	}

	float Math::inverseLerp(float start, float end, float f) {
		return (f - start) / (end - start);
	}

	float Math::lerp(float start, float end, float f) {
		if (f <= 0.0f) return start;
		if (f >= 1.0f) return end;

		return start + (end - start) * f;
	}

	float Math::lerpAngle(float start, float end, float f) {
		return fmodf(lerp(start, end, f), 360.0);
	}

	float Math::lerpUnclamped(float start, float end, float f) {
		return start + (end - start) * f;
	}

	float Math::fMax(float a, float b) {
		return fmaxf(a, b);
	}

	float Math::fMin(float a, float b) {
		return fminf(a, b);
	}

	float Math::moveTowards(float start, float end, float maxDelta) {
		if (end - start >= maxDelta) return start + maxDelta;
		return end;
	}

	float Math::moveTowardsAngle(float start, float end, float maxDelta) {
		return fmodf(moveTowards(start, end, maxDelta), 360.0f);
	}

	float Math::pingPong(float value, float max) {
		int n = (int)(value / max);
		if (n % 2 == 0) return fmodf(value, max);
		return max - fmodf(value, max);
	}

	float Math::pow(float f, float power) {
		return powf(f, power);
	}

	float Math::round(float f) {
		return roundf(f);
	}

	int Math::roundToInt(float f) {
		return (int)roundf(f);
	}

	int Math::sign(float f) {
		if (f > 0) return 1;
		if (f < 0) return -1;
		return 0;
	}

	float Math::sin(float f) {
		return sinf(f);
	}

	float Math::sqrt(float f) {
		return sqrtf(f);
	}

	float Math::tan(float f) {
		return tanf(f);
	}
}