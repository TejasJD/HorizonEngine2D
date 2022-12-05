#include "pch.h"

#include "RendererAPI.h"
#include "VertexArray.h"

#include "Platform/OpenGL/GLVertexArray.h"

namespace Hzn
{
	/// <summary>
	/// Function to create a vertex array.
	/// return pointer to GLVertexArray if OpenGl case is selected.
	/// </summary>
	/// <returns></returns>
	std::shared_ptr<VertexArray> VertexArray::create()
	{
		switch (RendererAPI::getAPI())
		{
		case RendererAPI::API::None:
			HZN_CORE_ASSERT(false, "No Render API Selected");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<GLVertexArray>();
		}

		HZN_CORE_ASSERT(false, "Invalid API selected!");
		return nullptr;
	}
}