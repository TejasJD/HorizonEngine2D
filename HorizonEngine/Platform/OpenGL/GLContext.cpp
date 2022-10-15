#include "pch.h"
#include "GLContext.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

//template<>
//struct fmt::formatter<const GLubyte*> : fmt::ostream_formatter {};

namespace Hzn
{

	GLContext::GLContext(GLFWwindow *const& handle) : m_Handle(handle)
	{
		HZN_CORE_ASSERT(m_Handle, "Handle is NULL");
	}

	void GLContext::init()
	{
		glfwMakeContextCurrent(m_Handle);
		
		int success = (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
		HZN_CORE_ASSERT(success, "failed to intialize GLAD!");

		HZN_CORE_INFO("Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		HZN_CORE_INFO("Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		HZN_CORE_INFO("Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
	
	}

	void GLContext::swapBuffers()
	{
		glfwSwapBuffers(m_Handle);
	}

}