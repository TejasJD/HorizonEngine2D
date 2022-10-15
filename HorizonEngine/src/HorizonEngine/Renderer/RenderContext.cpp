#include "pch.h"

#include "Renderer.h"
#include "RenderContext.h"

#include "Platform/OpenGL/GLContext.h"

namespace Hzn
{
	RenderContext* RenderContext::create(void* const& windowHandle)
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::None:
			HZN_CORE_ASSERT(false, "No Render API Selected");
			return nullptr;
		case RendererAPI::OpenGL:
			return new GLContext((GLFWwindow*)windowHandle);
		}

		HZN_CORE_ASSERT(false, "Invalid API selected!");
		return nullptr;
	}
}