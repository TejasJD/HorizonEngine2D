#include "pch.h"

#include "HorizonEngine/Renderer/Renderer.h"

namespace Hzn
{
	//! calls init the rendering, goes from Renderer.cpp -> RenderCall.cpp -> GLRenderAPI.cpp
	void Renderer::init()
	{
		RenderCall::init();
	}

	//! Resizes the window
	void Renderer::onWindowResize(uint32_t width, uint32_t height)
	{
		RenderCall::setViewport(0, 0, width, height);
	}
}