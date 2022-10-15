#pragma once

#ifndef HZN_VERTEX_ARRAY_H
#define HZN_VERTEX_ARRAY_H

namespace Hzn
{
	class VertexArray
	{
	public:
		virtual ~VertexArray() {}

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		static VertexArray* create();
	};
}

#endif // !HZN_VERTEX_ARRAY_H
