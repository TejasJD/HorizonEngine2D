#include "pch.h"

#include <entt/entt.hpp>
#include <box2d/b2_world_callbacks.h>
#include <box2d/b2_contact.h>

#include "HorizonEngine/Scene/SceneManager.h"
#include "HorizonEngine/Scene/GameObject.h"
#include "HorizonEngine/Scene/FunctionRegistry.h"

#include "HorizonEngine/Physics2D/ContactListener.h"

namespace Hzn {
void ContactListener::BeginContact(b2Contact* contact) {
  b2BodyUserData& bodyUserDataA =
      contact->GetFixtureA()->GetBody()->GetUserData();
  b2BodyUserData& bodyUserDataB =
      contact->GetFixtureB()->GetBody()->GetUserData();

  entt::entity A = entt::null, B = entt::null;

  if (bodyUserDataA.pointer) {
    A = *(entt::entity*)(bodyUserDataA.pointer);
  }

  if (bodyUserDataB.pointer) {
    B = *(entt::entity*)(bodyUserDataB.pointer);
  }

  uint32_t A_uint = entt::to_integral(A);
  uint32_t B_uint = entt::to_integral(B);

  if (contact->GetFixtureB()->IsSensor()) {
    if (g_TriggerEnterFunctionMap.find(A_uint) !=
        g_TriggerEnterFunctionMap.end()) {
      g_TriggerEnterFunctionMap.at(A_uint)(B_uint);
    }
  } else {
    if (g_CollisionEnterFunctionMap.find(A_uint) !=
        g_CollisionEnterFunctionMap.end()) {
      g_CollisionEnterFunctionMap.at(A_uint)(B_uint);
    }
  }

  if (contact->GetFixtureA()->IsSensor()) {
    if (g_TriggerEnterFunctionMap.find(B_uint) !=
        g_TriggerEnterFunctionMap.end()) {
      g_TriggerEnterFunctionMap.at(B_uint)(A_uint);
    }
  } else {
    if (g_CollisionEnterFunctionMap.find(B_uint) !=
        g_CollisionEnterFunctionMap.end()) {
      g_CollisionEnterFunctionMap.at(B_uint)(A_uint);
    }
  }
}

void ContactListener::EndContact(b2Contact* contact) {
  b2BodyUserData& bodyUserDataA =
      contact->GetFixtureA()->GetBody()->GetUserData();
  b2BodyUserData& bodyUserDataB =
      contact->GetFixtureB()->GetBody()->GetUserData();

  entt::entity A, B;
  if (bodyUserDataA.pointer) {
    A = *(entt::entity*)(bodyUserDataA.pointer);
  }

  if (bodyUserDataB.pointer) {
    B = *(entt::entity*)(bodyUserDataB.pointer);
  }

  uint32_t A_uint = entt::to_integral(A);
  uint32_t B_uint = entt::to_integral(B);

  if (contact->GetFixtureB()->IsSensor()) {
    if (g_TriggerExitFunctionMap.find(A_uint) !=
        g_TriggerExitFunctionMap.end()) {
      g_TriggerExitFunctionMap.at(A_uint)(B_uint);
    }
  } else {
    if (g_CollisionExitFunctionMap.find(A_uint) !=
        g_CollisionExitFunctionMap.end()) {
      g_CollisionExitFunctionMap.at(A_uint)(B_uint);
    }
  }

  if (contact->GetFixtureA()->IsSensor()) {
    if (g_TriggerExitFunctionMap.find(B_uint) !=
        g_TriggerExitFunctionMap.end()) {
      g_TriggerExitFunctionMap.at(B_uint)(A_uint);
    }
  } else {
    if (g_CollisionExitFunctionMap.find(B_uint) !=
        g_CollisionExitFunctionMap.end()) {
      g_CollisionExitFunctionMap.at(B_uint)(A_uint);
    }
  }
}
}  // namespace Hzn