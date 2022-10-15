#include "pch.h"

#include "Renderer.h"
#include "Shader.h"

#include "Platform/OpenGL/GLShader.h"

namespace Hzn
{
	Shader* Shader::create(const std::string& vertexSource, const std::string& fragmentSource)
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::None:
			HZN_CORE_ASSERT(false, "No Render API Selected");
			return nullptr;
		case RendererAPI::OpenGL:
			return new GLShader(vertexSource, fragmentSource);
		}

		HZN_CORE_ASSERT(false, "Invalid API selected!");
		return nullptr;
	}
}