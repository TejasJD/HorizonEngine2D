#include "pch.h"

#include "Renderer.h"

#include "Buffer.h"

#include "Platform/OpenGL/GLBuffer.h"

namespace Hzn
{
	VertexBuffer* VertexBuffer::create(std::vector<float> vertices)
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::None: 
			HZN_CORE_ASSERT(false, "No Render API Selected");
			return nullptr;
		case RendererAPI::OpenGL: 
			return new GLVertexBuffer(vertices.size() * sizeof(float), &vertices[0]);
		}

		HZN_CORE_ASSERT(false, "Invalid API selected!");
		return nullptr;
	}

	ElementBuffer* ElementBuffer::create(std::vector<unsigned int> indices)
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::None:
			HZN_CORE_ASSERT(false, "No Render API Selected");
			return nullptr;
		case RendererAPI::OpenGL:
			return new GLElementBuffer(indices.size(), &indices[0]);
		}

		HZN_CORE_ASSERT(false, "Invalid API selected!");
		return nullptr;
	}
}