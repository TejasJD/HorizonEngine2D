#pragma once

#ifndef HZN_GL_TEXTURE_H
#define HZN_GL_TEXTURE_H

#include "HorizonEngine/Renderer/Texture.h"

typedef unsigned int GLenum;

namespace Hzn
{
	class GLTexture2D : public Texture2D
	{
	public:
		GLTexture2D(uint32_t width, uint32_t height);
		GLTexture2D(const std::string& path);
		virtual ~GLTexture2D();

		virtual bool operator==(const Texture& rhs) const override
		{
			return m_TextureId == dynamic_cast<const GLTexture2D&>(rhs).m_TextureId;
		}

		virtual bool operator!=(const Texture& rhs) const override
		{
			return !(*this == rhs);
		}

		virtual unsigned int getWidth() const override { return m_Width; }
		virtual unsigned int getHeight() const override { return m_Height; }
		virtual void bind(uint32_t slot = 0) const override;
		void unbind() const override;
		void setData(void* data, uint32_t size) override;
		virtual uint32_t getId() const override { return m_TextureId; }
		virtual const std::string& getPath() const override { return m_Path; }

	private:
		std::string m_Path;
		uint32_t m_Width = 0, m_Height = 0;
		uint32_t m_TextureId = 0;
		GLenum m_Dataformat, m_Internalformat;
	};
}

#endif