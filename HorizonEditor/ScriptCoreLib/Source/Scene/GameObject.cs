
using System;

namespace Hzn
{
    public class GameObject
    {
        protected GameObject() { ID = 0; }

        internal GameObject(ulong id)
        {
            ID = id;
        }

        public readonly ulong ID;

        public Vector3 Translation
        {
            get
            {
                InternalCalls.TransformComponent_GetTranslation(ID, out Vector3 result);
                return result;
            }
            set
            {
                InternalCalls.TransformComponent_SetTranslation(ID, ref value);
            }
        }

        public bool HasComponent<T>() where T : Component, new()
        {
            Type componentType = typeof(T);
            return InternalCalls.GameObject_HasComponent(ID, componentType);
        }

        public T GetComponent<T>() where T : Component, new()
        {
            if (!HasComponent<T>())
                return null;

            T component = new T() { GameObject = this };
            return component;
        }

        public GameObject GetGameObjectByName(string name)
        {
            ulong GameObjectID = InternalCalls.GameObject_GetGameObjectByName(name);
            if (GameObjectID == 0)
                return null;

            return new GameObject(GameObjectID);
        }


        public T As<T>() where T : GameObject, new()
        {
            object instance = InternalCalls.GetScriptInstance(ID);
            return instance as T;
        }

    }
}