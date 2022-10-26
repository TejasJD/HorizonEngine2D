#include "pch.h"

#include "RendererAPI.h"
#include "Shader.h"

#include "Platform/OpenGL/GLShader.h"

namespace Hzn
{
	std::shared_ptr<Shader> Shader::create(const std::initializer_list<std::pair<ShaderType, std::string>>& nameAndPath)
	{
		switch (RendererAPI::getAPI())
		{
		case RendererAPI::API::None:
			HZN_CORE_ASSERT(false, "No Render API Selected");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<GLShader>(nameAndPath);
		}

		HZN_CORE_ASSERT(false, "Invalid API selected!");
		return nullptr;
	}

	std::shared_ptr<Shader> Shader::create(const std::string& vertexSource, const std::string& fragmentSource)
	{
		switch (RendererAPI::getAPI())
		{
		case RendererAPI::API::None:
			HZN_CORE_ASSERT(false, "No Render API Selected");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<GLShader>(vertexSource, fragmentSource);
		}

		HZN_CORE_ASSERT(false, "Invalid API selected!");
		return nullptr;
	}
}