#include "ContactListener.h"

#include <entt/entt.hpp>
#include <box2d/b2_world_callbacks.h>
#include <box2d/b2_contact.h>

#include "../SceneManagement/SceneManager.h"
#include "../SceneManagement/GameObject.h"

namespace Hzn {
    void ContactListener::BeginContact(b2Contact* contact) 
    {

        b2BodyUserData& bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
        b2BodyUserData& bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();

        entt::entity A, B;
        if (bodyUserDataA.pointer) {
            A = *(entt::entity*)(bodyUserDataA.pointer);
        }

        if (bodyUserDataB.pointer) {
            B = *(entt::entity*)(bodyUserDataB.pointer);
        }

        GameObject objA = SceneManager::getActiveScene()->getGameObjectById(entt::to_integral(A));
        GameObject objB = SceneManager::getActiveScene()->getGameObjectById(entt::to_integral(B));

        if (contact->GetFixtureA()->IsSensor()) objA.onTriggerEnter(objB);
        else objA.onCollisionEnter(objB);

        if (contact->GetFixtureB()->IsSensor()) objB.onTriggerEnter(objA);
        objB.onCollisionEnter(objA);
    }

    void ContactListener::EndContact(b2Contact* contact) 
    {

    }

    void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
    { /* handle pre-solve event */
    }

    void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
    { /* handle post-solve event */
    }
}