#pragma once


#ifndef HZN_SHADER_H
#define HZN_SHADER_H

#include <glm/glm.hpp>

namespace Hzn
{
	enum class ShaderType : uint8_t
	{
		None = 0, VertexShader, FragmentShader
	};

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		virtual unsigned int getId() const = 0;
		virtual void setUniform(const std::string& s, const glm::mat4& mat4) = 0;
		virtual void setUniform(const std::string& s, const glm::mat3& mat3) = 0;
		virtual void setUniform(const std::string& s, const glm::vec4& vec4) = 0;
		virtual void setUniform(const std::string& s, const glm::vec3& vec3) = 0;
		virtual void setUniform(const std::string& s, const glm::vec2& vec2) = 0;
		virtual void setUniform(const std::string& s, int ui) = 0;
		virtual void setUniform(const std::string& s, float uf) = 0;

		static std::shared_ptr<Shader> create(const std::initializer_list<std::pair<ShaderType, std::string>>& nameAndPath);
		static std::shared_ptr<Shader> create(const std::string& vertexSource, const std::string& fragmentSource);
	};
}

#endif // !HZN_SHADER_H