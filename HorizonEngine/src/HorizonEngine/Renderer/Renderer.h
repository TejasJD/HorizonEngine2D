#pragma once

#ifndef HZN_RENDERER_H
#define HZN_RENDERER_H

#include "VertexArray.h"
#include "RenderCall.h"

namespace Hzn
{
	class Renderer
	{
	public:
		static void beginScene();
		static void endScene();

		static void render(const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API getAPI() { return RendererAPI::getAPI(); }

	private:


	};
}

#endif // !HZN_RENDERER_H
