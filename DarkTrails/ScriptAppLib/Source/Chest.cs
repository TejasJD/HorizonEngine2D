using Hzn;
using System;
using System.Collections.Generic;

namespace Hzn {
    class Chest : GameObject {
        static uint playerId = 1;

        public void OnCreate() { }

        public void OnUpdate(float ts) { }

        public void OnCollisionEnter(uint otherId) {
            if (otherId == playerId) {
                if (PlayerPlatformer.numberOfKeys > 0) {
                    PlayerPlatformer.numberOfKeys--;
                    PlayerPlatformer.score++;

                    Console.WriteLine("Score: " + PlayerPlatformer.score);
                    Console.WriteLine("Remaining keys: " + PlayerPlatformer.numberOfKeys);

                    GameObject obj = this;
                    scene.DestroyGameObject(ref obj);
                }
            }
        }
    }
}
