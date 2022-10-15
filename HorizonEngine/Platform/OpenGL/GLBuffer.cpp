#include "pch.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "GLBuffer.h"

namespace Hzn
{
	//! ---------- GL VERTEX BUFFER MEMBER FUNCTION DEFINITIONS ----------
	
	GLVertexBuffer::GLVertexBuffer(const unsigned int& size, float* const& vertices)
	{
		glCreateBuffers(1, &m_VertexBufferId);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferId);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	GLVertexBuffer::~GLVertexBuffer()
	{
		glDeleteBuffers(1, &m_VertexBufferId);
	}

	void GLVertexBuffer::bind() const 
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferId);
	}

	void GLVertexBuffer::unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//! ---------- GL ELEMENT BUFFER MEMBER FUNCTION DEFINITIONS ----------

	GLElementBuffer::GLElementBuffer(const unsigned int& elementCount, unsigned int* const& indices)
		:m_Count(elementCount)
	{
		glCreateBuffers(1, &m_ElementBufferId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBufferId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	}

	GLElementBuffer::~GLElementBuffer()
	{
		glDeleteBuffers(1, &m_ElementBufferId);
	}

	void GLElementBuffer::bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBufferId);
	}
	
	void GLElementBuffer::unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}