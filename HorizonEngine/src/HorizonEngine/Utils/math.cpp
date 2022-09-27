#include "math.h";

static float Math::abs(float f) {
	return fabs(f);
}

static float Math::acos(float f) {
	return acosf(f);
}

static float Math::asin(float f) {
	return asinf(f);
}

static float Math::atan(float f) {
	return atanf(f);
}

static float Math::atan2(float f) {
	return atan2f(f);
}

static float Math::ceil(float f) {
	return ceilf(f);
}

static int Math::ceilToInt(float f) {
	return (int)ceilf(f);
}

static float Math::clamp(float f, float min, float max) {
	if (f < min) return min;
	if (f > max) return max;
	return f;
}

static float Math::clamp01(float f) {
	return clamp(f, 0.0f, 1.0f);
}

static float Math::cos(float f) {
	return cosf(f);
}

static float Math::deltaAngle(float current, float target) {
	current = fmodf(current, 360.0f) + 360.0f;
	target = fmodf(target, 360.0f) + 360.0f;
	return Math.abs(current - target);
}

static float Math::exp(float power) {
	return expf(power);
}

static float Math::floor(float f) {
	return floorf(f);
}

static int Math::floorToInt(float f) {
	return (int)floorf(f);
}

static float Math::inverseLerp(float start, float end, float f) {
	return (f - start) / (end - start);
}

static float Math::lerp(float start, float end, float f) {
	if (f <= 0.0f) return start;
	if (f >= 1.0f) return end;

	return start + (end - start) * f;
}

static float Math::lerpAngle(float start, float end, float f) {
	return fmodf(Math.lerp(start, end, f), 360.0);
}

static float Math::lerpUnclamped(float start, float end, float f) {
	return start + (end - start) * f;
}

static float Math::max(float a, float b) {
	return fmaxf(a, b);
}

static float Math::min(float a, float b) {
	return fminf(a, b);
}

static float Math::moveTowards(float start, float end, float maxDelta) {
	if (end - start >= maxDelta) return start + maxDelta;
	return end;
}

static float Math::moveTowardsAngle(float start, float end, float maxDelta) {
	return fmodf(moveTowards(start, end, maxDelta), 360.0f);
}

static float Math::pingPong(float value, float power) {

}

static float Math::pow(float f, float power) {
	return powf(f, power);
}

static float Math::round(float f) {
	return roundf(f);
}

static int Math::roundToInt(float f) {
	return (int)roundf(f);
}

static int Math::sign(float f) {
	if (f > 0) return 1;
	if (f < 0) return -1;
	return 0;
}

static float Math::sin(float f) {
	return sinf(f);
}

static float Math::sqrt(float f) {
	return sqrtf(f);
}

static float Math::tan(float f) {
	return tanf(f);
}