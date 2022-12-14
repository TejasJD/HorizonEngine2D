using Hzn;
using System;

namespace Hzn {
    class Player : GameObject {
        private TransformComponent m_Transform;
        private RigidBody2DComponent m_Rigidbody;

        public static uint portalId = 66;
        static uint otherPlayerId = 65;

        public void OnCreate() {

            Console.WriteLine($"Player1.OnCreate - {ID}");

            m_Transform = GetComponent<TransformComponent>();
            m_Rigidbody = GetComponent<RigidBody2DComponent>();
        }

        bool appliedStartVelocity = false;
        public void OnUpdate(float ts) {
            if (HasComponent<RigidBody2DComponent>() && !appliedStartVelocity) {
                m_Rigidbody = GetComponent<RigidBody2DComponent>();
                m_Rigidbody.ApplyLinearImpulse(new Vector2(2f, 0f), true);
                appliedStartVelocity = true;
            }

            Vector3 velocity = Vector3.Zero;

            //Control direction according to keyboard input
            if (Input.IsKeyDown(KeyCode.Up)) {
                velocity.Y = 3.0f;
            }

            velocity *= 35.0f * ts;

            if (m_Rigidbody != null) {
                m_Rigidbody.ApplyLinearImpulse(velocity.XY, true);
            }
        }

        public void OnTriggerEnter(uint otherId) {
            if (otherId == portalId) {
                GameObject obj = scene.GetGameObjectByID(otherId);
                scene.DestroyGameObject(ref obj);

                CameraMovement.playerId = otherPlayerId;
                CameraMovement.player = scene.GetGameObjectByID(otherPlayerId);
            } else {
                GameObject obj = this;
                scene.DestroyGameObject(ref obj);
            }
        }
    }
}