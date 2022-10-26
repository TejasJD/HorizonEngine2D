#include "pch.h"
#include "Renderer.h"

namespace Hzn
{
	glm::mat4* Renderer::s_ViewMatrix = nullptr;
	glm::mat4* Renderer::s_ProjectionMatrix = nullptr;

	void Renderer::init()
	{
		RenderCall::init();
	}

	void Renderer::beginScene(Camera& camera)
	{
		s_ViewMatrix = &camera.getViewMatrix();
		s_ProjectionMatrix = &camera.getProjectionMatrix();
	}

	void Renderer::endScene()
	{

	}

	void Renderer::render(const std::shared_ptr<Shader>& shader, 
		const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& modelTransform)
	{

		shader->bind();
		shader->setUniform("view", *s_ViewMatrix);
		shader->setUniform("projection", *s_ProjectionMatrix);
		shader->setUniform("model", modelTransform);

		vertexArray->bind();
		RenderCall::drawElements(vertexArray);
	}
}