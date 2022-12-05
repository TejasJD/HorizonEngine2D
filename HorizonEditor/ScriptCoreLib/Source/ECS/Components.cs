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
    }

    public class RigidBody2DComponent : Component
    {

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