#include "pch.h"
#include "Renderer.h"

namespace Hzn
{
	//! calls init the rendering, goes from Renderer.cpp -> RenderCall.cpp -> GLRenderAPI.cpp
	void Renderer::init()
	{
		RenderCall::init();
	}

	//! Resizes the window
	void Renderer::onWindowResize(unsigned int width, unsigned int height)
	{
		RenderCall::setViewport(0, 0, width, height);
	}
}