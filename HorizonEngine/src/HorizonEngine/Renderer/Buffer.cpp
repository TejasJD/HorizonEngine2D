#include "pch.h"

#include "Renderer.h"

#include "Buffer.h"

#include "Platform/OpenGL/GLBuffer.h"

namespace Hzn
{
	VertexBuffer* VertexBuffer::create(const unsigned int& size, float* const& vertices)
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::None: 
			HZN_CORE_ASSERT(false, "No Render API Selected");
			return nullptr;
		case RendererAPI::OpenGL: 
			return new GLVertexBuffer(size, vertices);
		}

		HZN_CORE_ASSERT(false, "Invalid API selected!");
		return nullptr;
	}

	ElementBuffer* ElementBuffer::create(const unsigned int& elementCount, unsigned int* const& indices)
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::None:
			HZN_CORE_ASSERT(false, "No Render API Selected");
			return nullptr;
		case RendererAPI::OpenGL:
			return new GLElementBuffer(elementCount, indices);
		}

		HZN_CORE_ASSERT(false, "Invalid API selected!");
		return nullptr;
	}
}