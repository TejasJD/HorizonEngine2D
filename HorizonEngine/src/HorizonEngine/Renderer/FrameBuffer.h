#pragma once

#ifndef HZN_FRAMEBUFFER_H
#define HZN_FRAMEBUFFER_H

namespace Hzn
{
	enum class FrameBufferTextureFormat
	{
		None = 0,
		RGBA8 = 1,
		RED_INTEGER = 2,
		DEPTH24_STENCIL8 = 3
	};

	struct FrameBufferTextureSpecification
	{
		FrameBufferTextureSpecification() = default;
		FrameBufferTextureSpecification(FrameBufferTextureFormat format)
			:m_Format(format) {}

		~FrameBufferTextureSpecification() = default;

		FrameBufferTextureFormat m_Format = FrameBufferTextureFormat::None;
	};

	struct FrameBufferAttachmentSpecification
	{
		FrameBufferAttachmentSpecification() = default;

		FrameBufferAttachmentSpecification(std::initializer_list<FrameBufferTextureSpecification> attachments)
			: m_Attachments(attachments) {}

		~FrameBufferAttachmentSpecification() = default;

		std::vector<FrameBufferTextureSpecification> m_Attachments;
	};

	/// <summary>
	///	Create a FrameBufferProperties object which holds the
	///	properties required to create a frame buffer.
	/// </summary>
	struct FrameBufferProps
	{
		uint32_t width = 0;
		uint32_t height = 0;
		FrameBufferAttachmentSpecification attachments;
		uint32_t samples = 1;
		bool isBind = false;
	};

	/// <summary>
	/// Use frame buffers when you want to render your data to textures,
	///	or render objects.
	///	</summary>
	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;

		/// <summary>
		/// Get the properties of the frame buffer object. Is read-only.
		///	</summary>
		///	<returns>Returns a const reference to the properties of the object.</returns>
		virtual const FrameBufferProps& getProps() const = 0;

		/// <summary>
		/// Bind the frame buffer. We bind the frame buffer in every iteration of the
		/// application loop before we start doing any Render calls.
		///	</summary>
		virtual void bind() = 0;

		/// <summary>
		/// Unbind the frame buffer. We unbind the frame buffer in every iteration of the
		///	application after we have made all the Render calls.
		///	</summary>
		virtual void unbind() = 0;

		/// <summary>
		/// Re-creates the frame buffer. Invalidates the previous frame buffer and generates
		///	a new one.
		///	</summary>
		///	<param name="width">Width of the frame buffer.</param>
		///	<param name="height">Height of the frame buffer.</param>
		virtual void recreate(uint32_t width, uint32_t height) = 0;

		/// <summary>
		/// Get the Id Color buffer that is attached to the frame buffer.
		///	This is usually Id of the texture that is used as a color attachment
		///	for the frame buffer.
		///	</summary>
		///	<returns>integer with denotes the color attachment Id.</returns>
		virtual uint32_t getColorAttachmentId(uint32_t index = 0) const = 0;

		/// <summary>
		/// Creates a frame buffer object from the properties in
		///	the object of 'FrameBufferProps'.
		///	</summary>
		///	<param name="props">object of 'FrameBufferProps'.</param>
		static std::shared_ptr<FrameBuffer> create(const FrameBufferProps& props);
	};

}

#endif