#include "pch.h"

#include "RendererAPI.h"
#include "VertexArray.h"

#include "Platform/OpenGL/GLVertexArray.h"

namespace Hzn
{
	VertexArray* VertexArray::create()
	{
		switch (RendererAPI::getAPI())
		{
		case RendererAPI::API::None:
			HZN_CORE_ASSERT(false, "No Render API Selected");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new GLVertexArray();
		}

		HZN_CORE_ASSERT(false, "Invalid API selected!");
		return nullptr;
	}
}