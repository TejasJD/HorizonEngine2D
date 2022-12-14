using Hzn;
using System;
using System.Collections.Generic;

namespace Hzn {
    class Key : GameObject {
        static uint playerId = 1;

        public void OnCreate() { }

        public void OnUpdate(float ts) { }

        public void OnCollisionEnter(uint otherId) {
            if (otherId == playerId) {
                PlayerPlatformer.numberOfKeys++;
                Console.WriteLine("Remaining keys: " + PlayerPlatformer.numberOfKeys);

                GameObject obj = this;
                scene.DestroyGameObject(ref obj);
            }
        }
    }
}
