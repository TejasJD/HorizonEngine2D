#include "pch.h"
#include "Renderer.h"

namespace Hzn
{
	std::shared_ptr<Camera> Renderer::m_Camera;

	void Renderer::beginScene(const std::shared_ptr<Camera>& camera)
	{
		m_Camera = camera;
	}

	void Renderer::endScene()
	{

	}

	void Renderer::render(const std::shared_ptr<Shader>& shader, 
		const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& modelTransform)
	{
		if (!m_Camera)
		{
			throw std::runtime_error("Camera not bound!");
		}

		shader->bind();
		shader->setUniform("view", m_Camera->getViewMatrix());
		shader->setUniform("projection", m_Camera->getPerspectiveProjectionMatrix(0.1f, 100.0f));
		shader->setUniform("model", modelTransform);

		vertexArray->bind();
		RenderCall::drawElements(vertexArray);
		/*RenderCall::drawTriangles(vertexArray);*/
	}
}