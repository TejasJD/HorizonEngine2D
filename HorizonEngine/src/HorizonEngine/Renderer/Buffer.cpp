#include "pch.h"

#include "RendererAPI.h"
#include "Buffer.h"

#include "Platform/OpenGL/GLBuffer.h"

namespace Hzn
{
	//!Use when data is known
	std::shared_ptr<VertexBuffer> VertexBuffer::create(float* vertices, uint32_t size)
	{
		//!Switch case for openGL and any other RendererAPI
		switch (RendererAPI::getAPI())
		{
		case RendererAPI::API::None: 
			HZN_CORE_ASSERT(false, "No Render API Selected");
			return nullptr;
		case RendererAPI::API::OpenGL: 
			return std::make_shared<GLVertexBuffer>(vertices, size);
		}

		HZN_CORE_ASSERT(false, "Invalid API selected!");
		return nullptr;
	}

	//!Use when data is not known
	std::shared_ptr<VertexBuffer> VertexBuffer::create(uint32_t size)
	{
		//!Switch case for openGL and any other RendererAPI
		switch (RendererAPI::getAPI())
		{
		case RendererAPI::API::None:
			HZN_CORE_ASSERT(false, "No Render API Selected");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<GLVertexBuffer>(size);
		}

		HZN_CORE_ASSERT(false, "Invalid API selected!");
		return nullptr;
	}

	//! Element Buffer Creation
	std::shared_ptr<ElementBuffer> ElementBuffer::create(uint32_t* indices, uint32_t count)
	{
		switch (RendererAPI::getAPI())
		{
		case RendererAPI::API::None:
			HZN_CORE_ASSERT(false, "No Render API Selected");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<GLElementBuffer>(indices, count);
		}

		HZN_CORE_ASSERT(false, "Invalid API selected!");
		return nullptr;
	}
}