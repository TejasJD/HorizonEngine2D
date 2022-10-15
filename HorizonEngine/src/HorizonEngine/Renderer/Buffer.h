#pragma once

#ifndef HZN_BUFFER_H
#define HZN_BUFFER_H

namespace Hzn
{
	//! Interface for implementing concrete Vertex Buffers.
	//! Concrete implementation is bound at the runtime.
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {};
		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		static VertexBuffer* create(const unsigned int& sizeInBytes, float* const& vertices);

	protected:
		VertexBuffer() {}

	};

	//! Interface for implementing concrete Element Buffers.
	//! Concrete implementation is bound at the runtime.
	class ElementBuffer
	{
	public:
		virtual ~ElementBuffer() {};
		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		virtual unsigned int size() const = 0;

		static ElementBuffer* create(const unsigned int& elementCount, unsigned int* const& indices);

	protected:
		ElementBuffer() {}

	};
}

#endif // !HZN_VERTEX_BUFFER_H
