#pragma once

#ifndef HZN_GL_RENDERER_API_H
#define HZN_GL_RENDERER_API_H

#include "HorizonEngine/Renderer/RendererAPI.h"

namespace Hzn
{
	class GLRendererAPI : public RendererAPI
	{
	public:
		GLRendererAPI();
		virtual ~GLRendererAPI();
		// Inherited via RendererAPI
		virtual void setClearColor(const glm::vec4& color) override;
		virtual void submitClear() override;
		virtual void drawElements(const std::shared_ptr<VertexArray>& vertexArray) override;
		virtual void drawTriangles(const std::shared_ptr<VertexArray>& vertexArray) override;
	};
}

#endif // !HZN_GL_RENDERER_API_H
