#include "pch.h"
#include "HorizonEngine/Renderer/RendererAPI.h"
#include "HorizonEngine/Renderer/Framebuffer.h"

#include "OpenGL/GLFramebuffer.h"

namespace Hzn
{
	//! Implementation of the Framebuffer create
	std::shared_ptr<Framebuffer> Framebuffer::create(const FramebufferProps& props)
	{
		switch (RendererAPI::getAPI())
		{
		case RendererAPI::API::None:
			HZN_CORE_ASSERT(false, "No Renderer API Selected");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<GLFramebuffer>(props);
		}

		HZN_CORE_ASSERT(false, "Invalid Renderer API");
		return nullptr;
	}
}