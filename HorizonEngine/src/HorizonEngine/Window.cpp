#include "pch.h"

#include "HorizonEngine/Renderer/RendererAPI.h"
#include "Window.h"

#include "Platform/OpenGL/GLWindow.h"

namespace Hzn
{
	//! creates the window for the HorizonGE
	std::shared_ptr<Window> Window::create(uint32_t width, uint32_t height, const char* title)
	{
		//! switch case for rendering the openGL if selected
		switch (RendererAPI::getAPI())
		{
		case RendererAPI::API::None:
			HZN_CORE_ASSERT(false, "No Render API Selected");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<GLWindow>(width, height, title);
		}

		//! no API has been selected
		HZN_CORE_ASSERT(false, "Invalid API selected!");
		return nullptr;
	}
}