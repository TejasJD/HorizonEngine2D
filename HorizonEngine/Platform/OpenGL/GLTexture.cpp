#include "pch.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"

#include "GLTexture.h"

namespace Hzn {

GLTexture2D::GLTexture2D(const std::string& path) : m_Path(path) {
  int width, height, channels;
  HZN_CORE_WARN("{0}", path);
  stbi_set_flip_vertically_on_load(1);
  stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
  HZN_CORE_ASSERT(data, "Failed to load image!");
  m_Width = width;
  m_Height = height;

  GLenum internalformat = 0, dataformat = 0;

  if (channels == 4) {
    internalformat = GL_RGBA8;
    dataformat = GL_RGBA;
  } else if (channels == 3) {
    internalformat = GL_RGB8;
    dataformat = GL_RGB;
  }
  m_Internalformat = internalformat;
  m_Dataformat = dataformat;

  glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureId);
  glTextureStorage2D(m_TextureId, 1, m_Internalformat, m_Width, m_Height);

  glTextureParameteri(m_TextureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTextureParameteri(m_TextureId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTextureParameteri(m_TextureId, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTextureParameteri(m_TextureId, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTextureSubImage2D(m_TextureId, 0, 0, 0, m_Width, m_Height, m_Dataformat,
                      GL_UNSIGNED_BYTE, data);

  stbi_image_free(data);
}

GLTexture2D::GLTexture2D(uint32_t width, uint32_t height)
    : m_Width(width), m_Height(height) {
  m_Internalformat = GL_RGBA8;
  m_Dataformat = GL_RGBA;

  glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureId);
  glTextureStorage2D(m_TextureId, 1, m_Internalformat, m_Width, m_Height);

  glTextureParameteri(m_TextureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTextureParameteri(m_TextureId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTextureParameteri(m_TextureId, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTextureParameteri(m_TextureId, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

GLTexture2D::~GLTexture2D() { glDeleteTextures(1, &m_TextureId); }

void GLTexture2D::bind(uint32_t slot) const {
  glBindTextureUnit(slot, m_TextureId);
}

void GLTexture2D::unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }

void GLTexture2D::setData(void* data, uint32_t size) {
  glTextureSubImage2D(m_TextureId, 0, 0, 0, m_Width, m_Height, m_Dataformat,
                      GL_UNSIGNED_BYTE, data);
}

}  // namespace Hzn