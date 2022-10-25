#pragma once

#ifndef HZN_RENDERER_H
#define HZN_RENDERER_H

#include "VertexArray.h"
#include "HorizonEngine/Camera.h"
#include "Shader.h"
#include "RenderCall.h"

namespace Hzn
{
	class Renderer
	{
	public:
		static void init();
		static void beginScene(const std::shared_ptr<Camera>& camera);
		static void endScene();
		static void render(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray,
			const glm::mat4& modelTransform = glm::mat4(1.0f));
		inline static RendererAPI::API getAPI() { return RendererAPI::getAPI(); }

	private:
		static std::shared_ptr<Camera> m_Camera;
	};
}

#endif // !HZN_RENDERER_H
