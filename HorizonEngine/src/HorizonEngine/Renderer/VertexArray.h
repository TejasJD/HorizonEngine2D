#pragma once

#ifndef HZN_VERTEX_ARRAY_H
#define HZN_VERTEX_ARRAY_H

#include "Buffer.h"

namespace Hzn
{
	class VertexArray
	{
	public:
		virtual ~VertexArray() {}

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
		virtual void setElementBuffer(const std::shared_ptr<ElementBuffer>& elementBuffer) = 0;

		virtual std::shared_ptr<ElementBuffer> getElementBuffer() = 0;
		virtual std::vector<std::shared_ptr<VertexBuffer>> getVertexBuffers() = 0;

		static std::shared_ptr<VertexArray> create();
	};
}

#endif // !HZN_VERTEX_ARRAY_H
