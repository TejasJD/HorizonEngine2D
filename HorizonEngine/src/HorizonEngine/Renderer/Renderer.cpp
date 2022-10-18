#include "pch.h"
#include "Renderer.h"

namespace Hzn
{
	void Renderer::beginScene()
	{
	}

	void Renderer::endScene()
	{

	}

	void Renderer::render(const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->bind();
		RenderCall::drawElements(vertexArray);
	}
}