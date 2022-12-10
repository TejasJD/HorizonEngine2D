using System;
using System.Runtime.CompilerServices;

namespace Hzn
{
    public static class InternalCalls
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void Greet();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsKeyDown(KeyCode keycode);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static object GetScriptInstance(uint gameObjectID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool GameObject_HasComponent(uint gameObjectID, Type componentType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GameObject_AddComponent(uint gameObjectID, Type componentType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GameObject_RemoveComponent(uint gameObjectID, Type componentType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static string GameObject_GetScene(uint gameObjectID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static uint Scene_CreateGameObject(string name);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static uint Scene_GetGameObjectByID(uint gameObjectID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Scene_DestroyGameObject(uint gameObjectID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static string NameComponent_GetName(uint gameObjectID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void NameComponent_SetName(uint gameObjectID, string name);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static uint RelationComponent_GetParent(uint gameObjectID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetTranslation(uint gameObjectID, out Vector3 translation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_SetTranslation(uint gameObjectID, ref Vector3 translation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetRotation(uint gameObjectID, out Vector3 rotation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_SetRotation(uint gameObjectID, ref Vector3 rotation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetScale(uint gameObjectID, out Vector3 scale);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_SetScale(uint gameObjectID, ref Vector3 scale);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RenderComponent_GetColor(uint gameObjectID, out Vector4 scale);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RenderComponent_SetColor(uint gameObjectID, ref Vector4 scale);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody2DComponent_GetLinearVelocity(uint gameObjectID, out Vector2 velocity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody2DComponent_SetLinearVelocity(uint gameObjectID, ref Vector2 velocity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody2DComponent_GetAngle(uint gameObjectID, out float angle);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody2DComponent_ApplyLinearImpulse(uint gameObjectID, ref Vector2 impulse, ref Vector2 point, bool wake);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody2DComponent_ApplyLinearImpulseToCenter(uint gameObjectID, ref Vector2 impulse, bool wake);


    }
}