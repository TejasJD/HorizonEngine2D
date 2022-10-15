#pragma once


#ifndef HZN_SHADER_H
#define HZN_SHADER_H

namespace Hzn
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSource, const std::string& fragmentSource) {}
		virtual ~Shader() {}

		virtual void use() const = 0;
	};
}

#endif // !HZN_SHADER_H