#pragma once

#ifndef HZN_RENDER_COMMAND_H
#define HZN_RENDER_COMMAND_H

#include "RendererAPI.h"

namespace Hzn
{
	class RenderCall
	{
	public:

		inline static void setClearColor(const glm::vec4& color)
		{
			m_Api->setClearColor(color);
		}

		inline static void submitClear()
		{
			m_Api->submitClear();
		}

		inline static void drawElements(const std::shared_ptr<VertexArray>& vertexArray)
		{
			m_Api->drawElements(vertexArray);
		}

		inline static void drawTriangles(const std::shared_ptr<VertexArray>& vertexArray)
		{
			m_Api->drawTriangles(vertexArray);
		}

		static RendererAPI* init();

	private:
		static RendererAPI* m_Api;
	};
}

#endif