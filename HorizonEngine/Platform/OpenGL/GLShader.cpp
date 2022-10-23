#include "pch.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/type_ptr.hpp"

#include "GLShader.h"

namespace Hzn
{
	// reference: https://www.khronos.org/opengl/wiki/Shader_Compilation
	GLShader::GLShader(const std::string& vertexSource, const std::string& fragmentSource)
	{
		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = (const GLchar*)vertexSource.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.

			// In this simple program, we'll just leave
			HZN_CORE_ERROR("{0}", infoLog.data());
			HZN_CORE_ASSERT(isCompiled, "Vertex Shader Compilation Failed!");
			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = (const GLchar*)fragmentSource.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.

			// In this simple program, we'll just leave
			HZN_CORE_ERROR("{0}", infoLog.data());
			HZN_CORE_ASSERT(isCompiled, "Fragment Shader Compilation Failed!");
			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		m_ProgramId = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(m_ProgramId, vertexShader);
		glAttachShader(m_ProgramId, fragmentShader);

		// Link our m_ProgramId
		glLinkProgram(m_ProgramId);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(m_ProgramId, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_ProgramId, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_ProgramId, maxLength, &maxLength, &infoLog[0]);

			// We don't need the m_ProgramId anymore.
			glDeleteProgram(m_ProgramId);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			// Use the infoLog as you see fit.

			// In this simple m_ProgramId, we'll just leave
			HZN_CORE_ERROR("{0}", infoLog.data());
			HZN_CORE_ASSERT(isLinked, "Shader Linking Failed!");
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(m_ProgramId, vertexShader);
		glDetachShader(m_ProgramId, fragmentShader);
	}

	GLShader::~GLShader()
	{
		glDeleteProgram(m_ProgramId);
	}

	//! calling this member function uses the shader.
	void GLShader::bind() const
	{
		glUseProgram(m_ProgramId);
	}

	void GLShader::unbind() const
	{

	}

	void GLShader::setUniform(const std::string& s, const glm::mat4& mat4)
	{
		uint32_t uLoc = glGetUniformLocation(m_ProgramId, s.c_str());
		glUniformMatrix4fv(uLoc, 1, GL_FALSE, glm::value_ptr(mat4));
	}
	void GLShader::setUniform(const std::string& s, const glm::mat3& mat3)
	{
		uint32_t uLoc = glGetUniformLocation(m_ProgramId, s.c_str());
		glUniformMatrix3fv(uLoc, 1, GL_FALSE, glm::value_ptr(mat3));
	}
	void GLShader::setUniform(const std::string& s, const glm::vec4& vec4)
	{
		uint32_t uLoc = glGetUniformLocation(m_ProgramId, s.c_str());
		glUniform4fv(uLoc, 1, glm::value_ptr(vec4));
	}
	void GLShader::setUniform(const std::string& s, const glm::vec3& vec3)
	{
		uint32_t uLoc = glGetUniformLocation(m_ProgramId, s.c_str());
		glUniform3fv(uLoc, 1, glm::value_ptr(vec3));
	}
	void GLShader::setUniform(const std::string& s, const glm::vec2& vec2)
	{
		uint32_t uLoc = glGetUniformLocation(m_ProgramId, s.c_str());
		glUniform2fv(uLoc, 1, glm::value_ptr(vec2));
	}
}