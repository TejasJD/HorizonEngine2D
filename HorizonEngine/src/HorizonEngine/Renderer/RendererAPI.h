#pragma once

#ifndef HZN_RENDERER_API_H
#define HZN_RENDERER_API_H

#include "glm/glm.hpp"
#include "VertexArray.h"

namespace Hzn
{
	class RendererAPI
	{
	public:
		virtual ~RendererAPI() {}
		enum class API
		{
			None = 0,
			OpenGL = 1
		};

		inline static RendererAPI::API getAPI() { return m_Api; }
		
		virtual void init() = 0;
		virtual void setClearColor(const glm::vec4& color) = 0;
		virtual void submitClear() = 0;
		virtual void drawElements(const std::shared_ptr<VertexArray>& vertexArray) = 0;
		virtual void setViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) = 0;

	private:
		static API m_Api;
	};

}
#endif // !HZN_RENDERER_API
