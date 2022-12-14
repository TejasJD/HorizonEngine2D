using Hzn;
using System;
using System.Collections.Generic;

namespace Hzn {
    class PipeSpawner : GameObject {
        public static List<GameObject> pipes = new List<GameObject>();
        public static int numberOfPipes = 10;

        public void OnCreate() {
            for (int i = 0; i < numberOfPipes; i++) {
                CreatePipe((2 + 4 * i), -1.5f + (float)new Random().NextDouble() * 3f);
            }

            GameObject portal = scene.CreateGameObject("Portal");
            portal.Translation = new Vector3(pipes[pipes.Count - 1].Translation.X,
                                             pipes[pipes.Count - 1].Translation.Y - 5f,
                                             pipes[pipes.Count - 1].Translation.Z);
            portal.GetComponent<TransformComponent>().Scale = new Vector3(0.5f, 2f, 1f);
            portal.AddComponent<RenderComponent>();
            portal.GetComponent<RenderComponent>().Color = new Vector4(0f, 1f, 0f, 1f);
            portal.AddComponent<RigidBody2DComponent>();
            portal.AddComponent<BoxCollider2DComponent>();
            portal.GetComponent<BoxCollider2DComponent>().Sensor = true;
            Player.portalId = portal.ID;
        }

        public void OnUpdate(float ts) {

        }

        void CreatePipe(float x, float y) {
            GameObject pipeDown = scene.CreateGameObject("PipeDown");
            pipeDown.Translation = new Vector3(x, y - 5f, 0);
            pipeDown.GetComponent<TransformComponent>().Scale = new Vector3(2f, 5f, 1f);
            pipeDown.AddComponent<RenderComponent>();
            pipeDown.GetComponent<RenderComponent>().Color = new Vector4(1f, 0f, 0f, 1f);
            pipeDown.AddComponent<RigidBody2DComponent>();
            pipeDown.AddComponent<BoxCollider2DComponent>();
            pipeDown.GetComponent<BoxCollider2DComponent>().Sensor = true;

            GameObject pipeUp = scene.CreateGameObject("PipeUp");
            pipeUp.Translation = new Vector3(x, y + 5f, 0);
            pipeUp.GetComponent<TransformComponent>().Scale = new Vector3(2f, 5f, 1f);
            pipeUp.AddComponent<RenderComponent>();
            pipeUp.GetComponent<RenderComponent>().Color = new Vector4(1f, 0f, 0f, 1f);
            pipeUp.AddComponent<RigidBody2DComponent>();
            pipeUp.AddComponent<BoxCollider2DComponent>();
            pipeUp.GetComponent<BoxCollider2DComponent>().Sensor = true;

            pipes.Add(pipeDown);
            pipes.Add(pipeUp);
        }
    }
}
