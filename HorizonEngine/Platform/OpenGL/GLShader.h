#pragma once

#ifndef HZN_GL_SHADER_H
#define HZN_GL_SHADER_H

#include "HorizonEngine/Renderer/Shader.h"

namespace Hzn
{
	class GLShader : public Shader
	{
	public:
		GLShader(const std::string& vertexSource, const std::string& fragmentSource);
		~GLShader();

		// Inherited via Shader
		virtual void use() const override;

	private:
		unsigned int m_ProgramId;
	};
}


#endif //! HZN_GL_SHADER_H