#include "pch.h"
#include "GLRendererAPI.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Hzn
{
	void GLRendererAPI::init()
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void GLRendererAPI::setClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void GLRendererAPI::submitClear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GLRendererAPI::drawElements(const std::shared_ptr<VertexArray>& vertexArray, uint32_t count)
	{
		int elements = (count == 0) ? vertexArray->getElementBuffer()->size() : count;
		glDrawElements(GL_TRIANGLES, elements, GL_UNSIGNED_INT, nullptr);
	}

	void GLRendererAPI::setViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
	{
		glViewport(x, y, width, height);
	}
}