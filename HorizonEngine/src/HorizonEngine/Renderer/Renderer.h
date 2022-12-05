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
		//! Calss to init th renderer from the supplying graphics library (OpenGL)
		static void init();
		//! Resizes the window
		static void onWindowResize(unsigned int width, unsigned int height);
		//! retrieves the renderAPI
		inline static RendererAPI::API getAPI() { return RendererAPI::getAPI(); }
	};
}

#endif // !HZN_RENDERER_H
