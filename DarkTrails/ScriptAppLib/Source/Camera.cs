using Hzn;
using System;
using System.Collections.Generic;

namespace Hzn {
    class CameraMovement : GameObject {
        public static GameObject player;
        public static uint playerId = 1;

        public void OnCreate() {
            player = scene.GetGameObjectByID(playerId);
        }

        public void OnUpdate(float ts) {
            if (scene.GetGameObjectByID(playerId) != null) {
                if (!player.HasComponent<RigidBody2DComponent>()) {
                    player.AddComponent<RigidBody2DComponent>();
                    player.AddComponent<BoxCollider2DComponent>();
                    player.GetComponent<BoxCollider2DComponent>().Sensor = false;
                }


                TransformComponent t = GetComponent<TransformComponent>();
                t.Translation = new Vector3(player.Translation.X + 2f, t.Translation.Y, t.Translation.Z);
            }
        }
    }
}
