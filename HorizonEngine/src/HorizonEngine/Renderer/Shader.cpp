#include "pch.h"

#include "RendererAPI.h"
#include "Shader.h"

#include "Platform/OpenGL/GLShader.h"

namespace Hzn
{
	/// <summary>
	/// Provides a switch case to cater to scenario where openGL isn't the shader Used
	/// otherwise calls the GLShader class
	/// </summary>
	/// <param name="nameAndPath"></param>
	/// <returns></returns>
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
	/// <summary>
	/// Provides a switch case to cater to scenario where openGL isn't the shader Used
	/// otherwise calls the GLShader class
	/// Takes in the the source of the vertex & fragment sources as parameters
	/// </summary>
	/// <param name="vertexSource"></param>
	/// <param name="fragmentSource"></param>
	/// <returns></returns>
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