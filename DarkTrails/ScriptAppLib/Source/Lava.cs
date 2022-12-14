using Hzn;
using System;
using System.Collections.Generic;

namespace Hzn {
    class Lava : GameObject {
        static uint playerId = 1;

        public void OnCollisionEnter(uint otherId) {
            if (otherId == playerId) {
                Console.WriteLine("You tried to swim in lava. Unfortunatelly, you still don't have this skill :(");
                Console.WriteLine("Score: " + PlayerPlatformer.score);
            }

            GameObject obj = scene.GetGameObjectByID(otherId);
            scene.DestroyGameObject(ref obj);
        }

        public void OnTriggerEnter(uint otherId) {
            OnCollisionEnter(otherId);
        }
    }
}