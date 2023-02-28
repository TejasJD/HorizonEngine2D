#pragma once

#ifndef HZN_FRAMEBUFFER_H
#define HZN_FRAMEBUFFER_H

#include <string>
#include <cstdint>
#include <initializer_list>
#include <vector>

namespace Hzn
{

	enum class FramebufferTextureFormat
	{
		None = 0,
		RGBA8 = 1,
		RED_INTEGER = 2,
		DEPTH24_STENCIL8 = 3
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			:m_Format(format) {}

		~FramebufferTextureSpecification() = default;

		FramebufferTextureFormat m_Format = FramebufferTextureFormat::None;
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;

		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			: m_Attachments(attachments) {}

		~FramebufferAttachmentSpecification() = default;

		std::vector<FramebufferTextureSpecification> m_Attachments;
	};

	/// <summary>
	///	Create a FramebufferProperties object which holds the
	///	properties required to create a frame buffer.
	/// </summary>
	struct FramebufferProps
	{
		int32_t width = 0;
		int32_t height = 0;
		FramebufferAttachmentSpecification attachments;
		uint32_t samples = 1;
		bool isBind = false;
	};

	/// <summary>
	/// Use frame buffers when you want to render your data to textures,
	///	or render objects.
	///	</summary>
	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		/// <summary>
		/// Get the properties of the frame buffer object. Is read-only.
		///	</summary>
		///	<returns>Returns a const reference to the properties of the object.</returns>
		virtual const FramebufferProps& getProps() const = 0;

		/// <summary>
		/// Bind the frame buffer. We bind the frame buffer in every iteration of the
		/// application loop before we start doing any Render calls.
		///	</summary>
		virtual void bind() const = 0;

		/// <summary>
		/// Unbind the frame buffer. We unbind the frame buffer in every iteration of the
		///	application after we have made all the Render calls.
		///	</summary>
		virtual void unbind() const = 0;

		/// <summary>
		/// Re-creates the frame buffer. Invalidates the previous frame buffer and generates
		///	a new one.
		///	</summary>
		///	<param name="width">Width of the frame buffer.</param>
		///	<param name="height">Height of the frame buffer.</param>
		virtual void recreate(int32_t width, int32_t height) = 0;

		/// <summary>
		/// Get the Id Color buffer that is attached to the frame buffer.
		///	This is usually Id of the texture that is used as a color attachment
		///	for the frame buffer.
		///	</summary>
		///	<returns>integer with denotes the color attachment Id.</returns>
		virtual uint32_t getColorAttachmentId(uint32_t index = 0) const = 0;

		virtual int32_t readPixel(uint32_t attachmentIndex, int x, int y) const = 0;

		virtual void clearColorAttachment(uint32_t attachmentIndex, int value) const = 0;

		/// <summary>
		/// Creates a frame buffer object from the properties in
		///	the object of 'FramebufferProps'.
		///	</summary>
		///	<param name="props">object of 'FramebufferProps'.</param>
		static std::shared_ptr<Framebuffer> create(const FramebufferProps& props);
	};

}

#endif