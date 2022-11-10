#pragma once

#ifndef HZN_GL_FRAMEBUFFER_H
#define HZN_GL_FRAMEBUFFER_H

#include "HorizonEngine/Renderer/FrameBuffer.h"

namespace Hzn
{
	class GLFrameBuffer : public FrameBuffer
	{
	public:
		GLFrameBuffer(const FrameBufferProps& props);
		virtual ~GLFrameBuffer();

		virtual uint32_t getColorAttachmentId() const override { return m_ColorAttachment; }
		virtual void bind() override;
		virtual void unbind() override;
		virtual const FrameBufferProps& getProps() const override { return m_Props; }
		virtual void recreate(uint32_t width, uint32_t height) override;

	private:
		void invalidate();
		void destroy();
		uint32_t m_FrameBufferId = 0;
		uint32_t m_ColorAttachment = 0;
		uint32_t m_DepthAttachment = 0;
		FrameBufferProps m_Props;

	};
}

#endif