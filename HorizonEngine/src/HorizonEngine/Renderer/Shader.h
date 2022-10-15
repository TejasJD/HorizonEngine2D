#pragma once


#ifndef HZN_SHADER_H
#define HZN_SHADER_H

namespace Hzn
{
	class Shader
	{
	public:
		virtual ~Shader() {}

		virtual void use() const = 0;

		static Shader* create(const std::string& vertexSource, const std::string& fragmentSource);

	protected:
		Shader() {}
	};
}

#endif // !HZN_SHADER_H