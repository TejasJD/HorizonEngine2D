#include "pch.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"

#include "GLTexture.h"

namespace Hzn
{
	GLTexture2D::GLTexture2D(const std::string& path) 
		: m_Path(path) 
	{
		int width, height, channels;
		HZN_CORE_WARN("{0}", path);
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		HZN_CORE_ASSERT(data, "Failed to load image!");

		m_Width = width;
		m_Height = height;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureId);
		glTextureStorage2D(m_TextureId, 1, GL_RGB8, m_Width, m_Height);

		glTextureParameteri(m_TextureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_TextureId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(m_TextureId, 0, 0, 0, m_Width, m_Height, GL_RGB, GL_UNSIGNED_BYTE, data);
		
		stbi_image_free(data);
	}

	GLTexture2D::~GLTexture2D()
	{
		glDeleteTextures(1, &m_TextureId);
	}

	void GLTexture2D::bind(unsigned int slot) const
	{
		glBindTextureUnit(slot, m_TextureId);
	}
}