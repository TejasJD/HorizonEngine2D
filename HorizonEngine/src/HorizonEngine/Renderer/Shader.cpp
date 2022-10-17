#include "pch.h"

#include "RendererAPI.h"
#include "Shader.h"

#include "Platform/OpenGL/GLShader.h"

namespace Hzn
{
	Shader* Shader::create(const std::string& vertexSource, const std::string& fragmentSource)
	{
		switch (RendererAPI::getAPI())
		{
		case RendererAPI::API::None:
			HZN_CORE_ASSERT(false, "No Render API Selected");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new GLShader(vertexSource, fragmentSource);
		}

		HZN_CORE_ASSERT(false, "Invalid API selected!");
		return nullptr;
	}
}