namespace Hzn
{
    public abstract class Component
    {
        public GameObject GameObject { get; internal set; }
    }

    public class TransformComponent : Component
    {
        public Vector3 Translation
        {
            get
            {
                InternalCalls.TransformComponent_GetTranslation(GameObject.ID, out Vector3 translation);
                return translation;
            }
            set
            {
                InternalCalls.TransformComponent_SetTranslation(GameObject.ID, ref value);
            }
        }

        public Vector3 Rotation
        {
            get
            {
                InternalCalls.TransformComponent_GetRotation(GameObject.ID, out Vector3 rotation);
                return rotation;
            }

            set
            {
                InternalCalls.TransformComponent_SetRotation(GameObject.ID, ref value);
            }
        }

        public Vector3 Scale
        {
            get
            {
                InternalCalls.TransformComponent_GetScale(GameObject.ID, out Vector3 scale);
                return scale;
            }

            set
            {
                InternalCalls.TransformComponent_SetScale(GameObject.ID, ref value);
            }
        }
    }

    public class RigidBody2DComponent : Component
    {
        public Vector2 LinearVelocity
        {
            get
            {
                InternalCalls.RigidBody2DComponent_GetLinearVelocity(GameObject.ID, out Vector2 velocity);
                return velocity;
            }
            set
            {
                InternalCalls.RigidBody2DComponent_SetLinearVelocity(GameObject.ID, ref value);
            }
        }

        public float Angle
        {
            get
            {
                InternalCalls.RigidBody2DComponent_GetAngle(GameObject.ID, out float angle);
                return angle;
            }
        }

        public void ApplyLinearImpulse(Vector2 impulse, Vector2 worldPosition, bool wake)
        {
            InternalCalls.RigidBody2DComponent_ApplyLinearImpulse(GameObject.ID, ref impulse, ref worldPosition, wake);
        }

        public void ApplyLinearImpulse(Vector2 impulse, bool wake)
        {
            InternalCalls.RigidBody2DComponent_ApplyLinearImpulseToCenter(GameObject.ID, ref impulse, wake);
        }

    }
}