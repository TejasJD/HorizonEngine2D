#include "pch.h"

#include "RendererAPI.h"
#include "Texture.h"

#include "Platform/OpenGL/GLTexture.h"

namespace Hzn
{
	// function that allows you to create texture.
	std::shared_ptr<Texture2D> Texture2D::create(const std::string& path)
	{
		switch (RendererAPI::getAPI())
		{
		case RendererAPI::API::None:
			HZN_CORE_ASSERT(false, "No Render API Selected");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<GLTexture2D>(path);
		}

		HZN_CORE_ASSERT(false, "Invalid API selected!");
		return nullptr;
	}

	std::shared_ptr<Hzn::Texture2D> Texture2D::create(uint32_t width, uint32_t height)
	{
		switch (RendererAPI::getAPI())
		{
		case RendererAPI::API::None:
			HZN_CORE_ASSERT(false, "No Render API Selected");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<GLTexture2D>(width, height);
		}

		HZN_CORE_ASSERT(false, "Invalid API selected!");
		return nullptr;
	}

}