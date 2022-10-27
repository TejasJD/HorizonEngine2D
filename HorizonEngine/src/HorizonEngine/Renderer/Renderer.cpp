#include "pch.h"
#include "Renderer.h"

namespace Hzn
{
	void Renderer::init()
	{
		RenderCall::init();
	}

	void Renderer::onWindowResize(unsigned int width, unsigned int height)
	{
		RenderCall::setViewport(0, 0, width, height);
	}
}