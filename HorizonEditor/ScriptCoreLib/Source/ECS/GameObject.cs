using System;
using System.Runtime.CompilerServices;

namespace Hzn
{
    public class GameObject
    {
        protected GameObject() { ID = System.UInt32.MaxValue; }

        internal GameObject(uint id)
        {
            ID = id;
        }

        public readonly uint ID;
        
        public Scene scene
        {
            get
            {
                string SceneName = InternalCalls.GameObject_GetScene(ID);
                return new Scene(SceneName);
            }
        }

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

        public bool AddComponent<T>() where T : Component, new()
        {
            if(!HasComponent<T>())
            {
                Type componentType = typeof(T);
                InternalCalls.GameObject_AddComponent(ID, componentType);
                return true;
            }
            return false;
        }

        public bool RemoveComponent<T>() where T : Component, new()
        {
            if(HasComponent<T>())
            {
                Type componentType = typeof(T);
                InternalCalls.GameObject_RemoveComponent(ID, componentType);
                return true;
            }
            return false;
        }

        public T As<T>() where T : GameObject, new()
        {
            object instance = InternalCalls.GetScriptInstance(ID);
            return instance as T;
        }

        public GameObject GetParent()
        {
            uint ParentID = InternalCalls.RelationComponent_GetParent(ID);
            if (ParentID == System.UInt32.MaxValue)
            {
                Console.WriteLine($"GameObject: {ParentID} has no parent!");
                return null;
            }
            return new GameObject(ParentID);
        }

        public bool HasParent()
        {
            uint ParentID = InternalCalls.RelationComponent_GetParent(ID);
            return ParentID != System.UInt32.MaxValue;
        }

    }
}