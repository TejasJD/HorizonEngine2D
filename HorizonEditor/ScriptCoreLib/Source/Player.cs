using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Hzn
{
    public class Player : GameObject
    {
        private TransformComponent m_Transform;
        private RigidBody2DComponent m_Rigidbody;

        public float Speed;
        public float Time = 0.0f;

        void OnCreate()
        {
            Console.WriteLine($"Player.OnCreate - {ID}");

            m_Transform = GetComponent<TransformComponent>();
            m_Rigidbody = GetComponent<RigidBody2DComponent>();
        }

        void OnUpdate(float ts)
        {
            Time += ts;

            float speed = Speed;
            Vector3 velocity = Vector3.Zero;

            //Control direction according to keyboard input
            if (Input.IsKeyDown(KeyCode.W))
            {
                Console.WriteLine($"");
                velocity.Y = 1.0f;
            }
            else if (Input.IsKeyDown(KeyCode.S))
            {
                velocity.Y = -1.0f;

            }

            if (Input.IsKeyDown(KeyCode.A))
            {
                velocity.X = -1.0f;

            }
            else if (Input.IsKeyDown(KeyCode.D))
            {
                velocity.X = 1.0f;

            }

            velocity *= speed * ts;

            m_Rigidbody.ApplyLinearImpulse(velocity.XY, true);

        }

    }
}