#include "pch.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "GLVertexArray.h"

namespace Hzn
{
	GLVertexArray::GLVertexArray()
	{
		glCreateVertexArrays(1, &m_VertexArrayId);
		glBindVertexArray(m_VertexArrayId);
	}

	GLVertexArray::~GLVertexArray()
	{
		glDeleteVertexArrays(1, &m_VertexArrayId);
	}

	void GLVertexArray::bind() const 
	{
		glBindVertexArray(m_VertexArrayId);
	}

	void GLVertexArray::unbind() const
	{
		glBindVertexArray(0);
	}
}