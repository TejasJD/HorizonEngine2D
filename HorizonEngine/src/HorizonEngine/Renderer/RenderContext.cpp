#include "pch.h"

#include "RendererAPI.h"
#include "RenderContext.h"

#include "Platform/OpenGL/GLContext.h"

namespace Hzn
{
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