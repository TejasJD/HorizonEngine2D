#pragma once

#ifndef HZN_RENDER_COMMAND_H
#define HZN_RENDER_COMMAND_H

#include "RendererAPI.h"

namespace Hzn
{
	//! class with static member functions that makes calls to the appropriate rendering API.
	class RenderCall
	{
	public:

		static void init() { m_Api->init(); }

		static void setViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
		{
			m_Api->setViewport(0, 0, width, height);
		}

		static void setClearColor(const glm::vec4& color) { m_Api->setClearColor(color); }

		static void submitClear() { m_Api->submitClear(); }

		static void drawElements(const std::shared_ptr<VertexArray>& vertexArray, uint32_t count = 0) 
		{ 
			m_Api->drawElements(vertexArray, count); 
		}

		static std::unique_ptr<RendererAPI> create();

	private:
		static std::unique_ptr<RendererAPI> m_Api;
	};
}

#endif