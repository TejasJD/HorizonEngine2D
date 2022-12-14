using Hzn;
using System;
using System.Collections.Generic;

namespace Hzn {
    class Flag : GameObject {
        public void OnCreate() {
            GetComponent<RigidBody2DComponent>().LinearVelocity = new Vector2(2f, 0f);
        }

        public void OnUpdate(float ts) { }

        public void OnCollisionEnter(uint otherId) {
            Console.WriteLine("Finish");

            GameObject camera = scene.GetGameObjectByID(0);
            camera.Translation = new Vector3(140f, 0f, 0f);

            GameObject obj = scene.GetGameObjectByID(otherId);
            scene.DestroyGameObject(ref obj);
        }
    }
}
