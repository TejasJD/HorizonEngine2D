using Hzn;
using System;
using System.Collections.Generic;

namespace Hzn {
    class Projectile : GameObject {
        public void OnCreate() { }

        public void OnUpdate(float ts) { }

        public void OnCollisionEnter(uint otherId) {
            GameObject obj = scene.GetGameObjectByID(otherId);
            scene.DestroyGameObject(ref obj);
        }
    }
}
