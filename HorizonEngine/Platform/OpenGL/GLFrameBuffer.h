#pragma once

#ifndef HZN_GL_FRAMEBUFFER_H
#define HZN_GL_FRAMEBUFFER_H

#include "HorizonEngine/Renderer/Framebuffer.h"

namespace Hzn
{
	class GLFramebuffer : public Framebuffer
	{
	public:
		GLFramebuffer(const FramebufferProps& props);
		virtual ~GLFramebuffer();

		virtual uint32_t getColorAttachmentId(uint32_t index = 0) const override
		{
			HZN_CORE_ASSERT(index < m_ColorAttachments.size(), "Index greater than size!");
			return m_ColorAttachments[index];
		}
		virtual void bind() const override;
		virtual void unbind() const override;
		virtual const FramebufferProps& getProps() const override { return m_Props; }
		virtual void recreate(int32_t width, int32_t height) override;
		virtual int32_t readPixel(uint32_t attachmentIndex, int x, int y) const override;
		virtual void clearColorAttachment(uint32_t attachmentIndex, int value) const override;

	private:
		void invalidate();
		void destroy();
		uint32_t m_FramebufferId = 0;
		FramebufferProps m_Props;


		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecs;
		FramebufferTextureSpecification m_DepthAttachmentSpecs;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment = 0;
	};
}

#endif