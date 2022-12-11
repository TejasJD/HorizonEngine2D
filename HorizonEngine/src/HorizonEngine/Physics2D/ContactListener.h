#pragma once

#ifndef HZN_CONTACT_LISTENER_H
#define HZN_CONTACT_LISTENER_H

#include <box2d/b2_world_callbacks.h>

namespace Hzn {
    class ContactListener : public b2ContactListener
    {
        virtual void BeginContact(b2Contact* contact) override;

        virtual void EndContact(b2Contact* contact) override;
    };
}

#endif // !CONTACT_LISTENR_H
