#include "pch.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "GLVertexArray.h"

namespace Hzn
{
	static GLenum ShaderDataTypeToGLenum(ShaderDataType type)
	{
		switch (type)
		{
		case Hzn::ShaderDataType::None: return GL_NONE;
		case Hzn::ShaderDataType::Vecf: return GL_FLOAT;
		case Hzn::ShaderDataType::Vec2f: return GL_FLOAT;
		case Hzn::ShaderDataType::Vec3f: return GL_FLOAT;
		case Hzn::ShaderDataType::Vec4f: return GL_FLOAT;
		case Hzn::ShaderDataType::Mat3f: return GL_FLOAT;
		case Hzn::ShaderDataType::Mat4f: return GL_FLOAT;
		case Hzn::ShaderDataType::Mat3i: return GL_INT;
		case Hzn::ShaderDataType::Mat4i: return GL_INT;
		case Hzn::ShaderDataType::Veci: return GL_INT;
		case Hzn::ShaderDataType::Vec2i: return GL_INT;
		case Hzn::ShaderDataType::Vec3i: return GL_INT;
		case Hzn::ShaderDataType::Vec4i: return GL_INT;
		case Hzn::ShaderDataType::Bool: return GL_BOOL;
		}

		HZN_CORE_ASSERT(false, "Unknown Shader Data Type!");
		return GL_NONE;
	}

	GLVertexArray::GLVertexArray()
	{
		glCreateVertexArrays(1, &m_VertexArrayId);
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

	void GLVertexArray::addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		glBindVertexArray(m_VertexArrayId);
		vertexBuffer->bind();
		unsigned int location = 0;

		for (const auto& element : vertexBuffer->getBufferLayout())
		{
			glVertexAttribPointer(
				location,
				element.getCount(),
				ShaderDataTypeToGLenum(element.m_Type),
				element.m_Normalized ? GL_TRUE : GL_FALSE,
				vertexBuffer->getBufferLayout().getStride(),
				reinterpret_cast<void*>(element.m_Offset)
			);

			glEnableVertexAttribArray(location);
			++location;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}
	void GLVertexArray::setElementBuffer(const std::shared_ptr<ElementBuffer>& elementBuffer)
	{
		glBindVertexArray(m_VertexArrayId);
		elementBuffer->bind();
		m_ElementBuffer = elementBuffer;
	}
}