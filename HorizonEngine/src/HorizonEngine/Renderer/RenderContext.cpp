#include "pch.h"

#include "HorizonEngine/Renderer/RendererAPI.h"
#include "HorizonEngine/Renderer/RenderContext.h"

#include "OpenGL/GLContext.h"

namespace Hzn
{
	//! Returns nullptr or the shared_ptr to the GLContext which owns the window handle
	std::shared_ptr<RenderContext> RenderContext::create(void* const& windowHandle)
	{
		switch (RendererAPI::getAPI())
		{
		case RendererAPI::API::None:
			HZN_CORE_ASSERT(false, "No Render API Selected");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<GLContext>((GLFWwindow*)windowHandle);
		}

		HZN_CORE_ASSERT(false, "Invalid API selected!");
		return nullptr;
	}
}