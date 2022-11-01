#pragma once

#ifndef HZN_GL_SHADER_H
#define HZN_GL_SHADER_H

#include "HorizonEngine/Renderer/Shader.h"

typedef unsigned int GLenum;

namespace Hzn
{
	class GLShader : public Shader
	{
	public:
		GLShader(const std::string& vertexSource, const std::string& fragmentSource);
		GLShader(const std::initializer_list<std::pair<ShaderType, std::string>>& typeAndPath);
		virtual ~GLShader();

		// Inherited via Shader
		virtual void bind() const override;
		virtual void unbind() const override;
		virtual unsigned int getId() const override { return m_ProgramId; }
		
		virtual void setUniform(const std::string& s, const glm::mat4& mat) override;
		virtual void setUniform(const std::string& s, const glm::mat3& mat3) override;
		virtual void setUniform(const std::string& s, const glm::vec4& vec4) override;
		virtual void setUniform(const std::string& s, const glm::vec3& vec3) override;
		virtual void setUniform(const std::string& s, const glm::vec2& vec2) override;
		virtual void setUniform(const std::string& s, int ui) override;
		virtual void setUniform(const std::string& s, int* a, uint32_t count) override;
		virtual void setUniform(const std::string& s, float uf) override;

		static GLenum HznShaderTypeToGLShader(ShaderType type);

	private:
		std::string readShaderFile(const std::string& filepath);
		void compileShaders(const std::string& vertexSource, const std::string& fragmentSource);
		unsigned int m_ProgramId;
	};
}


#endif //! HZN_GL_SHADER_H