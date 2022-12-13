#include "pch.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/type_ptr.hpp"

#include "GLShader.h"

namespace Hzn
{
	//! function which maps HorizonEngine Shader Type to GLShader Type.
	GLenum GLShader::HznShaderTypeToGLShader(ShaderType type)
	{
		switch (type)
		{
		case Hzn::ShaderType::None:
			HZN_CORE_ASSERT(false, "No Shader type provided.");
			return GL_NONE;
		case Hzn::ShaderType::VertexShader:
			return GL_VERTEX_SHADER;
		case Hzn::ShaderType::FragmentShader:
			return GL_FRAGMENT_SHADER;
		}

		HZN_CORE_ASSERT(false, "Invalid shader");
		return GL_NONE;
	}

	//! provide Shaders in terms of type and filepath. Only Vertex and Fragment Shaders supported for now!.
	GLShader::GLShader(const std::initializer_list<std::pair<ShaderType, std::string>>& typeAndPath)
	{
		if (typeAndPath.size() != 2)
		{
			HZN_CORE_ERROR("Inapproriate Shader configuration provided, vertex and fragment shaders supported only!");
			throw std::runtime_error("Vertex and Fragment shaders support only!");
		}

		std::string vertexSource, fragmentSource;

		for (const auto& shader : typeAndPath)
		{
			if (shader.first == ShaderType::VertexShader)
			{
				vertexSource = readShaderFile(shader.second);
			}
			else if (shader.first == ShaderType::FragmentShader)
			{
				fragmentSource = readShaderFile(shader.second);
			}
		}
		compileShaders(vertexSource, fragmentSource);
	}

	//! utility function that reads file the Shaderfile.
	std::string GLShader::readShaderFile(const std::string& filepath)
	{
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (!in)
		{
			throw std::runtime_error("Shader file not found!\n");
		}
		//! set file pointer to the end
		in.seekg(0, std::ios::end);
		//! get the position of the file pointer.
		uint32_t contentSize = in.tellg();

		// set the file pointer to the beginning again
		in.seekg(0, std::ios::beg);

		// allocate space for contents
		std::string contents;
		contents.resize(contentSize);

		in.read(&contents[0], contents.size());
		in.close();

		return contents;
	}

	// reference: https://www.khronos.org/opengl/wiki/Shader_Compilation
	GLShader::GLShader(const std::string& vertexSource, const std::string& fragmentSource)
	{
		compileShaders(vertexSource, fragmentSource);
	}

	
	void GLShader::compileShaders(const std::string& vertexSource, const std::string& fragmentSource)
	{
		//! Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		//! Send the vertex shader source code to GL
		//! Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = (const GLchar*)vertexSource.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		//! Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			//! The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			//! We don't need the shader anymore.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.

			// In this simple program, we'll just leave
			HZN_CORE_ERROR("{0}", infoLog.data());
			HZN_CORE_ASSERT(isCompiled, "Vertex Shader Compilation Failed!");
			return;
		}

		//! Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		//! Send the fragment shader source code to GL
		//! Note that std::string's .c_str is NULL character terminated.
		source = (const GLchar*)fragmentSource.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		//! Compile the fragment shader
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
	
	void GLShader::setUniform(const std::string& s, int ui)
	{
		uint32_t uLoc = glGetUniformLocation(m_ProgramId, s.c_str());
		glUniform1i(uLoc, ui);
	}
	
	void GLShader::setUniform(const std::string& s, float uf)
	{
		uint32_t uLoc = glGetUniformLocation(m_ProgramId, s.c_str());
		glUniform1f(uLoc, uf);
	}

	void GLShader::setUniform(const std::string& s, int* a, uint32_t count)
	{
		uint32_t uLoc = glGetUniformLocation(m_ProgramId, s.c_str());
		glUniform1iv(uLoc, count, a);
	}

}