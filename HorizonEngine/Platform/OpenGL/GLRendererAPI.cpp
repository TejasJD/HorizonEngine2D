#include "pch.h"
#include "GLRendererAPI.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Hzn
{
	GLRendererAPI::GLRendererAPI()
	{
	}

	GLRendererAPI::~GLRendererAPI()
	{

	}

	void GLRendererAPI::setClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void GLRendererAPI::submitClear()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void GLRendererAPI::drawElements(const std::shared_ptr<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->getElementBuffer()->size(), GL_UNSIGNED_INT, nullptr);
	}
}