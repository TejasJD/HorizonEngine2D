using Hzn;
using System;
using System.Collections.Generic;

namespace Hzn {
    internal class PlayerPlatformer : GameObject {
        public static bool canJump = true;

        public static int numberOfKeys = 0;
        public static int score = 0;

        private RigidBody2DComponent m_Rigidbody;

        public void OnCreate() {
            m_Rigidbody = GetComponent<RigidBody2DComponent>();
        }

        public void OnUpdate(float ts) {
            Vector3 velocity = Vector3.Zero;

            //Control direction according to keyboard input
            if (Input.IsKeyDown(KeyCode.Up) && canJump) {
                velocity.Y = 40.0f;
                canJump = false;
            }

            if (Input.IsKeyDown(KeyCode.Left)) {
                velocity.X = -2.0f;
            } else if (Input.IsKeyDown(KeyCode.Right)) {
                velocity.X = 2.0f;
            }

            velocity *= 35.0f * ts;

            if (m_Rigidbody != null) {
                m_Rigidbody.ApplyLinearImpulse(velocity.XY, true);
            }
        }

        public void OnCollisionEnter(uint otherId) {
            GameObject obj = scene.GetGameObjectByID(otherId);
            if (obj.GetComponent<NameComponent>().Name == "Enemy") {
                if (GetComponent<TransformComponent>().Translation.Y - obj.GetComponent<TransformComponent>().Translation.Y >= 0.9f) {
                    scene.DestroyGameObject(ref obj);
                }
            } else {
                canJump = true;
            }
        }

        public void OnTriggerEnter(uint otherId) { 
            GameObject obj = scene.GetGameObjectByID(otherId);
            if (obj.GetComponent<NameComponent>().Name == "Projectile") {
                Console.WriteLine("You couldn't dodge the sneaky attack and you died :(");
                Console.WriteLine("Score: " + score);

                GameObject player = this;
                scene.DestroyGameObject(ref player);
            }
        }
    }
}
