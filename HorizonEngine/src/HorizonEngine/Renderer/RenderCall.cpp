#include "pch.h"
#include "RenderCall.h"

#include "Platform/OpenGL/GLRendererAPI.h"

namespace Hzn
{
	std::unique_ptr<RendererAPI> RenderCall::m_Api = RenderCall::create();

	//!Create will return the render API - in this case OpenGL
	std::unique_ptr<RendererAPI> RenderCall::create()
	{
		switch (RendererAPI::getAPI())
		{
		case RendererAPI::API::None:
			HZN_CORE_ASSERT(false, "No Renderer API selected!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_unique<GLRendererAPI>();
		}

		HZN_CORE_ASSERT(false, "Invalid Renderer API!")
		return nullptr;
	}
}