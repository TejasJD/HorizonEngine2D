#pragma once

#ifndef HZN_CONTACT_LISTENER_H
#define HZN_CONTACT_LISTENER_H

#include <box2d/b2_world_callbacks.h>

namespace Hzn {
    class ContactListener : public b2ContactListener
    {
        void BeginContact(b2Contact* contact);

        void EndContact(b2Contact* contact);

        void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);

        void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
    };
}

#endif // !CONTACT_LISTENR_H
