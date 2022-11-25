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

		virtual uint32_t getColorAttachmentId(uint32_t index = 0) const override
		{
			HZN_CORE_ASSERT(index < m_ColorAttachments.size(), "Index greater than size!");
			return m_ColorAttachments[index];
		}
		virtual void bind() override;
		virtual void unbind() override;
		virtual const FrameBufferProps& getProps() const override { return m_Props; }
		virtual void recreate(int32_t width, int32_t height) override;

	private:
		void invalidate();
		void destroy();
		uint32_t m_FrameBufferId = 0;
		FrameBufferProps m_Props;


		std::vector<FrameBufferTextureSpecification> m_ColorAttachmentSpecs;
		FrameBufferTextureSpecification m_DepthAttachmentSpecs;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment = 0;
	};
}

#endif