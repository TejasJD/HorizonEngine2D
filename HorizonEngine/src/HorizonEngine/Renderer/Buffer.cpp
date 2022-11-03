#include "pch.h"

#include "RendererAPI.h"
#include "Buffer.h"

#include "Platform/OpenGL/GLBuffer.h"

namespace Hzn
{
	std::shared_ptr<VertexBuffer> VertexBuffer::create(float* vertices, uint32_t size)
	{
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

	std::shared_ptr<VertexBuffer> VertexBuffer::create(uint32_t size)
	{
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