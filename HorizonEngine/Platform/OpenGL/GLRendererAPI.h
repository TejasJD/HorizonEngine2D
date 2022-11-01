#pragma once

#ifndef HZN_GL_RENDERER_API_H
#define HZN_GL_RENDERER_API_H

#include "HorizonEngine/Renderer/RendererAPI.h"

namespace Hzn
{
	class GLRendererAPI : public RendererAPI
	{
	public:
		GLRendererAPI() {}
		virtual ~GLRendererAPI() {}

		// Inherited via RendererAPI
		virtual void init() override;
		virtual void setClearColor(const glm::vec4& color) override;
		virtual void submitClear() override;
		virtual void drawElements(const std::shared_ptr<VertexArray>& vertexArray, uint32_t count = 0) override;
		virtual void setViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) override;
	};
}

#endif // !HZN_GL_RENDERER_API_H
