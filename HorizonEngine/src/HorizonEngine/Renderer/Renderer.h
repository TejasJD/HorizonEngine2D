#pragma once

#ifndef HZN_RENDERER_H
#define HZN_RENDERER_H

#include "VertexArray.h"
#include "Buffer.h"
#include "Shader.h"
#include "Texture.h"
#include "RenderCall.h"
#include "HorizonEngine/Camera/Camera.h"

namespace Hzn
{
	class Renderer
	{
	public:
		static void init();
		static void onWindowResize(unsigned int width, unsigned int height);
		inline static RendererAPI::API getAPI() { return RendererAPI::getAPI(); }
	};
}

#endif // !HZN_RENDERER_H
