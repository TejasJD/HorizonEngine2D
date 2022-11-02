#include <pch.h>

#include "RendererAPI.h"
#include "FrameBuffer.h"

#include "Platform/OpenGL/GLFrameBuffer.h"

namespace Hzn
{
	std::shared_ptr<FrameBuffer> FrameBuffer::create(const FrameBufferProps& props)
	{
		switch (RendererAPI::getAPI())
		{
		case RendererAPI::API::None:
			HZN_CORE_ASSERT(false, "No Renderer API Selected");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<GLFrameBuffer>(props);
		}

		HZN_CORE_ASSERT(false, "Invalid Renderer API");
		return nullptr;
	}
}