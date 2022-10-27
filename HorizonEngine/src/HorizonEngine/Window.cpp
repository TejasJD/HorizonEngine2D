#include "pch.h"

#include "HorizonEngine/Renderer/RendererAPI.h"
#include "Window.h"

#include "Platform/OpenGL/GLWindow.h"

namespace Hzn
{
	std::shared_ptr<Window> Window::create(const unsigned int& width, const unsigned int& height, const char* const& title)
	{
		switch (RendererAPI::getAPI())
		{
		case RendererAPI::API::None:
			HZN_CORE_ASSERT(false, "No Render API Selected");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<GLWindow>(width, height, title);
		}

		HZN_CORE_ASSERT(false, "Invalid API selected!");
		return nullptr;
	}
}