namespace Hzn
{
    public abstract class Component
    {
        public GameObject GameObject { get; internal set; }
    }

    public class NameComponent : Component
    {
        public string Name
        {
            get
            {
                return InternalCalls.NameComponent_GetName(GameObject.ID);
            }
            set
            {
                InternalCalls.NameComponent_SetName(GameObject.ID, value);
            }
        }
    }

    public class RelationComponent : Component
    {
        public GameObject Parent
        {
            get
            {
                uint ParentID = InternalCalls.RelationComponent_GetParent(GameObject.ID);
                return new GameObject(ParentID);
            }
        }
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

    public class RenderComponent : Component
    {
        public Vector4 Color
        {
            get
            {
                InternalCalls.RenderComponent_GetColor(GameObject.ID, out Vector4 color);
                return color;
            }
            set
            {
                InternalCalls.RenderComponent_SetColor(GameObject.ID, ref value);
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

    public class BoxCollider2DComponent : Component
    {
        public bool Sensor
        {
            get
            {
                return InternalCalls.BoxCollider2DComponent_GetSensor(GameObject.ID);
            }
            set
            {
                InternalCalls.BoxCollider2DComponent_SetSensor(GameObject.ID, value);
            }
        }
    }
}