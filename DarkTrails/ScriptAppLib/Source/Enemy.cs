using Hzn;
using System;
using System.Collections.Generic;

namespace Hzn {
    class Enemy : GameObject {
        float attackTimer;

        public void OnCreate() {
            Random random = new Random();
            attackTimer = 1.0f + (float)random.NextDouble() * 2.0f;
        }

        public void OnUpdate(float ts) { 
            attackTimer -= ts;

            if (attackTimer <= 0.0f) {
                Random random = new Random();
                attackTimer = 1.0f + (float)random.NextDouble() * 2.0f;

                GameObject projectile = scene.CreateGameObject("Projectile");
                projectile.Translation = new Vector3(Translation.X, Translation.Y, 1.0f);
                projectile.GetComponent<TransformComponent>().Scale = new Vector3(0.5f, 0.5f, 1.0f);
                projectile.AddComponent<RenderComponent>();
                projectile.GetComponent<RenderComponent>().Color = new Vector4(0.8f, 0.8f, 0.8f, 1.0f);
                projectile.AddComponent<RigidBody2DComponent>();
                projectile.AddComponent<BoxCollider2DComponent>();
                projectile.GetComponent<BoxCollider2DComponent>().Sensor = true;
                projectile.GetComponent<RigidBody2DComponent>().ApplyLinearImpulse(new Vector2(-1.0f, 1.0f), true);
            }
        }
    }
}
