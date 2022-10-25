#pragma once

#ifndef HZN_GL_TEXTURE_H
#define HZN_GL_TEXTURE_H

#include "HorizonEngine/Renderer/Texture.h"

namespace Hzn
{
	class GLTexture2D : public Texture2D
	{
	public:
		GLTexture2D(const std::string& path);
		virtual ~GLTexture2D();

		virtual unsigned int getWidth() const override { return m_Width; }
		virtual unsigned int getHeight() const override { return m_Height; }

		virtual void bind(unsigned int slot = 0) const override;

	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_TextureId;
	};
}

#endif