using Hzn;
using System;
using System.Collections.Generic;

namespace Hzn {
    internal class Ground : GameObject {
        static uint playerId;

        public void OnCreate() { }

        public void OnUpdate(float ts) { }

        public void OnCollisionEnter(uint otherId) {
            if (otherId == playerId) {
                PlayerPlatformer.canJump = true;
            }
        }
    }
}
