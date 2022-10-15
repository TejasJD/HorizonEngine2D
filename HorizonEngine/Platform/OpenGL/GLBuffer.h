#pragma once

#ifndef HZN_GL_BUFFER_H
#define HZN_GL_BUFFER_H

#include "HorizonEngine/Renderer/Buffer.h"

namespace Hzn
{
	class GLVertexBuffer : public VertexBuffer 
	{
	public:
		GLVertexBuffer(const unsigned int& size, float* const& vertices);
		virtual ~GLVertexBuffer();

		virtual void bind() const override;
		virtual void unbind() const override;

	private:
		unsigned int m_VertexBufferId = 0;
	};

	class GLElementBuffer : public ElementBuffer
	{
	public:
		GLElementBuffer(const unsigned int& elementCount, unsigned int* const& indices);
		virtual ~GLElementBuffer();

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual unsigned int size() const override { return m_Count; }

	private:
		unsigned int m_Count = 0;
		unsigned int m_ElementBufferId = 0;
	};
}


#endif // !HZN_GL_BUFFER_H
