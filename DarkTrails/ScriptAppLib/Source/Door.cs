using Hzn;
using System;
using System.Collections.Generic;

namespace Hzn {
    class Door : GameObject {
        static uint playerId = 1;

        public void OnCreate() { }

        public void OnUpdate(float ts) { }

        public void OnCollisionEnter(uint otherId) {
            if (otherId == playerId) {
                if (PlayerPlatformer.numberOfKeys > 0) {
                    PlayerPlatformer.numberOfKeys--;
                    Console.WriteLine("Level completed!");

                    GameObject camera = scene.GetGameObjectByID(0);
                    camera.Translation = new Vector3(50f, 0f, 0f);

                    GameObject obj = this;
                    scene.DestroyGameObject(ref obj);
                }
            }
        }
    }
}

