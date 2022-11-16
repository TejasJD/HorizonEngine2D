#include "pch.h"
#include "GLFrameBuffer.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Hzn
{
	namespace Utils {

		static GLenum textureTarget(bool multisampled)
		{
			return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void createTextures(uint32_t* data, int count)
		{
			glGenTextures(count, data);
		}

		static void bindTexture(bool multisampled, uint32_t id)
		{
			glBindTexture(textureTarget(multisampled), id);
		}

		static void AttachColorTexture(
			uint32_t samples,
			uint32_t attachmentNumber,
			uint32_t id,
			uint32_t width,
			uint32_t height,
			FrameBufferTextureFormat format
		)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGBA8, width, height, GL_FALSE);
			}
			else
			{
				// apply the rigth format.

				if(format == FrameBufferTextureFormat::RGBA8)
				{
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
				}
				else if(format == FrameBufferTextureFormat::RED_INTEGER)
				{
					glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, nullptr);
				}
				else 
				{
					HZN_CORE_ASSERT(false, "No Color Texture format provided!");
				}

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentNumber, textureTarget(multisampled), id, 0);
		}

		static void AttachDepthTexture(uint32_t samples, uint32_t id, uint32_t width, uint32_t height, FrameBufferTextureFormat format)
		{
			bool multisampled = samples > 1;

			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_DEPTH24_STENCIL8, width, height, GL_FALSE);
			}
			else
			{
				// apply the rigth format.

				if (format == FrameBufferTextureFormat::DEPTH24_STENCIL8)
				{
					glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
				}
				else
				{
					HZN_CORE_ASSERT(false, "No Depth Texture format provided!");
				}

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}

			if (format == FrameBufferTextureFormat::DEPTH24_STENCIL8)
			{
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, textureTarget(multisampled), id, 0);
			}
		}


		static bool isDepthFormat(FrameBufferTextureFormat format)
		{
			switch (format)
			{
			case FrameBufferTextureFormat::DEPTH24_STENCIL8: return true;
			default: return false;
			}
		}
	}

	GLFrameBuffer::GLFrameBuffer(const FrameBufferProps& props)
		: m_Props(props)
	{
		for (auto attachments : m_Props.attachments.m_Attachments)
		{
			if (!Utils::isDepthFormat(attachments.m_Format))
			{
				m_ColorAttachmentSpecs.emplace_back(attachments);
			}
			else
			{
				m_DepthAttachmentSpecs = attachments;
			}
		}

		invalidate();
	}

	GLFrameBuffer::~GLFrameBuffer()
	{
		destroy();
	}

	void GLFrameBuffer::destroy()
	{
		glDeleteFramebuffers(1, &m_FrameBufferId);
		glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);
	}


	void GLFrameBuffer::invalidate()
	{
		if (m_FrameBufferId != 0)
		{
			destroy();
			m_ColorAttachments.clear();
			m_DepthAttachment = 0;
		}

		glGenFramebuffers(1, &m_FrameBufferId);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferId);

		bool multisample = m_Props.samples > 1;

		if (!m_ColorAttachmentSpecs.empty())
		{
			m_ColorAttachments.resize(m_ColorAttachmentSpecs.size());
			Utils::createTextures(m_ColorAttachments.data(), m_ColorAttachments.size());

			for (size_t i = 0; i < m_ColorAttachments.size(); ++i)
			{
				Utils::bindTexture(multisample, m_ColorAttachments[i]);
				Utils::AttachColorTexture(m_Props.samples, i, m_ColorAttachments[i], m_Props.width, m_Props.height,
					m_ColorAttachmentSpecs[i].m_Format);
			}
		}

		if(m_DepthAttachmentSpecs.m_Format != FrameBufferTextureFormat::None)
		{
			Utils::createTextures(&m_DepthAttachment, 1);
			Utils::bindTexture(multisample, m_DepthAttachment);
			Utils::AttachDepthTexture(m_Props.samples, m_DepthAttachment, m_Props.width, m_Props.height, m_DepthAttachmentSpecs.m_Format);
		}


		// for more color attachments.
		if(m_ColorAttachments.size() > 1)
		{
			HZN_CORE_ASSERT(m_ColorAttachments.size() <= 4, "not more than 4 color attachments supported!");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_ColorAttachments.size(), buffers);
		}
		else if (m_ColorAttachments.empty())
		{
			glDrawBuffer(GL_NONE);
		}


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
