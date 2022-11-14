#include "pch.h"
#include "GLFrameBuffer.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Hzn
{
	GLFrameBuffer::GLFrameBuffer(const FrameBufferProps& props)
		: m_Props(props)
	{
		invalidate();
	}

	GLFrameBuffer::~GLFrameBuffer()
	{
		destroy();
	}

	void GLFrameBuffer::destroy()
	{
		glDeleteFramebuffers(1, &m_FrameBufferId);
		glDeleteTextures(1, &m_ColorAttachment);
		glDeleteTextures(1, &m_DepthAttachment);
	}


	void GLFrameBuffer::invalidate()
	{
		if(m_FrameBufferId != 0)
		{
			destroy();
		}

		glGenFramebuffers(1, &m_FrameBufferId);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferId);

		// Color attachment
		glGenTextures(1, &m_ColorAttachment);
		glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Props.width, m_Props.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

		// Depth attachment. we will be sampling from texture as well.
		glGenTextures(1, &m_DepthAttachment);
		glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Props.width, m_Props.height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

		HZN_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Frame Buffer is incomplete");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void GLFrameBuffer::recreate(uint32_t width, uint32_t height)
	{
		if (width > 0 && height > 0)
		{
			m_Props.width = width;
			m_Props.height = height;
			invalidate();
		}
	}


	void GLFrameBuffer::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferId);
		glViewport(0, 0, m_Props.width, m_Props.height);
	}

	void GLFrameBuffer::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}
