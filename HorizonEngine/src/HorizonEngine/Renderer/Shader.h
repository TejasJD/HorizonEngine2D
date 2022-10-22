#pragma once


#ifndef HZN_SHADER_H
#define HZN_SHADER_H

#include <glm/glm.hpp>

namespace Hzn
{
	class Shader
	{
	public:
		virtual ~Shader() {}

		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		virtual unsigned int getId() const = 0;
		virtual void setUniform(const std::string& s, const glm::mat4& mat) = 0;

		static Shader* create(const std::string& vertexSource, const std::string& fragmentSource);
	};
}

#endif // !HZN_SHADER_H