#pragma once

#ifndef HZN_GL_VERTEX_ARRAY_H
#define HZN_GL_VERTEX_ARRAY_H

#include "HorizonEngine/Renderer/VertexArray.h"

namespace Hzn
{
	class GLVertexArray : public VertexArray
	{
	public:
		GLVertexArray();
		virtual ~GLVertexArray();

		virtual void bind() const override;
		virtual void unbind() const override;

	private:
		unsigned int m_VertexArrayId = 0;

	};
}

#endif // !HZN_GL_VERTEX_ARRAY_H
