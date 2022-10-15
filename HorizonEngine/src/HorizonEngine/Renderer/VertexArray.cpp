#include "pch.h"

#include "Renderer.h"
#include "VertexArray.h"

#include "Platform/OpenGL/GLVertexArray.h"

namespace Hzn
{
	VertexArray* VertexArray::create()
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::None:
			HZN_CORE_ASSERT(false, "No Render API Selected");
			return nullptr;
		case RendererAPI::OpenGL:
			return new GLVertexArray();
		}

		HZN_CORE_ASSERT(false, "Invalid API selected!");
		return nullptr;
	}
}