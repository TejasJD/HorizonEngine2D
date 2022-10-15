#pragma once

#include "HorizonEngine/Renderer/RenderContext.h"

#ifndef HZN_GL_CONTEXT_H
#define HZN_GL_CONTEXT_H

struct GLFWwindow;

namespace Hzn
{
	class GLContext : public RenderContext
	{
	public:
		GLContext(GLFWwindow *const& handle);

		// Inherited via RenderContext
		virtual void init() override;
		virtual void swapBuffers() override;

	private:
		GLFWwindow* m_Handle = nullptr;
	};
}

#endif // !HZN_GL_CONTEXT_H
