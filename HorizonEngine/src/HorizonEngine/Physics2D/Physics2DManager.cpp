#include "Physics2DManager.h"

namespace Hzn {
	b2Vec2 Physics2DManager::m_Gravity(0, -9.81);

	bool Physics2DManager::m_DoSleep = true;
	int32 Physics2DManager::m_VelocityIterations = 5;
	int32 Physics2DManager::m_PositionIterations = 3;

	b2World* Physics2DManager::m_World = nullptr;

	Physics2DContactListener Physics2DManager::contactListener;
}