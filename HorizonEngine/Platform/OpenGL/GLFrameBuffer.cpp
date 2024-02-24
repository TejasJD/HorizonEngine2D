#include "pch.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "HorizonEngine/Core/App.h"

#include "OpenGL/GLFramebuffer.h"

namespace Hzn {
namespace Utils {

static GLenum textureTarget(bool multisampled) {
  return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
}

static void createTextures(uint32_t* data, int count) {
  glGenTextures(count, data);
}

static void bindTexture(bool multisampled, uint32_t id) {
  glBindTexture(textureTarget(multisampled), id);
}

static void AttachColorTexture(uint32_t samples, uint32_t attachmentNumber,
                               uint32_t id, int32_t width, int32_t height,
                               FramebufferTextureFormat format) {
  bool multisampled = samples > 1;
  if (multisampled) {
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGBA8, width,
                            height, GL_FALSE);
  } else {
    // apply the rigth format.

    if (format == FramebufferTextureFormat::RGBA8) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA,
                   GL_UNSIGNED_BYTE, nullptr);
    } else if (format == FramebufferTextureFormat::RED_INTEGER) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, width, height, 0, GL_RED_INTEGER,
                   GL_UNSIGNED_BYTE, nullptr);
    } else {
      HZN_CORE_ASSERT(false, "No Color Texture format provided!");
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }

  glFramebufferTexture2D(GL_FRAMEBUFFER,
                         GL_COLOR_ATTACHMENT0 + attachmentNumber,
                         textureTarget(multisampled), id, 0);
}

static void AttachDepthTexture(uint32_t samples, uint32_t id, int32_t width,
                               int32_t height,
                               FramebufferTextureFormat format) {
  bool multisampled = samples > 1;

  if (multisampled) {
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples,
                            GL_DEPTH24_STENCIL8, width, height, GL_FALSE);
  } else {
    // apply the rigth format.

    if (format == FramebufferTextureFormat::DEPTH24_STENCIL8) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0,
                   GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
    } else {
      HZN_CORE_ASSERT(false, "No Depth Texture format provided!");
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }

  if (format == FramebufferTextureFormat::DEPTH24_STENCIL8) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                           textureTarget(multisampled), id, 0);
  }
}

static bool isDepthFormat(FramebufferTextureFormat format) {
  switch (format) {
    case FramebufferTextureFormat::DEPTH24_STENCIL8:
      return true;
    default:
      return false;
  }
}

static GLenum toGLTextureFormat(FramebufferTextureFormat format) {
  switch (format) {
    case FramebufferTextureFormat::RGBA8:
      return GL_RGBA8;
    case FramebufferTextureFormat::RED_INTEGER:
      return GL_RED_INTEGER;
    default: {
      HZN_CORE_ASSERT(false, "Invalid Texture Format");
      return GL_NONE;
    }
  }
}
}  // namespace Utils

GLFramebuffer::GLFramebuffer(const FramebufferProps& props) : m_Props(props) {
  for (auto attachments : m_Props.attachments.m_Attachments) {
    if (!Utils::isDepthFormat(attachments.m_Format)) {
      m_ColorAttachmentSpecs.emplace_back(attachments);
    } else {
      m_DepthAttachmentSpecs = attachments;
    }
  }

  invalidate();
}

GLFramebuffer::~GLFramebuffer() { destroy(); }

void GLFramebuffer::destroy() {
  glDeleteFramebuffers(1, &m_FramebufferId);
  glDeleteTextures(static_cast<int32_t>(m_ColorAttachments.size()),
                   m_ColorAttachments.data());
  glDeleteTextures(1, &m_DepthAttachment);
}

void GLFramebuffer::invalidate() {
  if (m_FramebufferId != 0) {
    destroy();
    m_ColorAttachments.clear();
    m_DepthAttachment = 0;
  }

  glGenFramebuffers(1, &m_FramebufferId);
  glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferId);

  bool multisample = m_Props.samples > 1;

  if (!m_ColorAttachmentSpecs.empty()) {
    m_ColorAttachments.resize(m_ColorAttachmentSpecs.size());
    Utils::createTextures(m_ColorAttachments.data(),
                          static_cast<int32_t>(m_ColorAttachments.size()));

    for (size_t i = 0; i < m_ColorAttachments.size(); ++i) {
      Utils::bindTexture(multisample, m_ColorAttachments[i]);
      Utils::AttachColorTexture(
          m_Props.samples, static_cast<uint32_t>(i), m_ColorAttachments[i],
          m_Props.width, m_Props.height, m_ColorAttachmentSpecs[i].m_Format);
    }
  }

  if (m_DepthAttachmentSpecs.m_Format != FramebufferTextureFormat::None) {
    Utils::createTextures(&m_DepthAttachment, 1);
    Utils::bindTexture(multisample, m_DepthAttachment);
    Utils::AttachDepthTexture(m_Props.samples, m_DepthAttachment, m_Props.width,
                              m_Props.height, m_DepthAttachmentSpecs.m_Format);
  }

  // for more color attachments.
  if (m_ColorAttachments.size() > 1) {
    HZN_CORE_ASSERT(m_ColorAttachments.size() <= 4,
                    "not more than 4 color attachments supported!");
    GLenum buffers[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
                         GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
    glDrawBuffers(static_cast<int32_t>(m_ColorAttachments.size()), buffers);
  } else if (m_ColorAttachments.empty()) {
    glDrawBuffer(GL_NONE);
  }

  HZN_CORE_ASSERT(
      glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
      "Frame Buffer is incomplete");

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLFramebuffer::recreate(int32_t width, int32_t height) {
  if (width > 0 && height > 0) {
    m_Props.width = width;
    m_Props.height = height;
    invalidate();
  }
}

int32_t GLFramebuffer::readPixel(uint32_t attachmentIndex, int32_t x,
                                 int32_t y) const {
  glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
  int32_t pixelData = 0;
  glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
  return pixelData;
}

void GLFramebuffer::clearColorAttachment(uint32_t attachmentIndex,
                                         int32_t value) const {
  HZN_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size(),
                  "attachment index out of bounds!");
  auto& spec = m_ColorAttachmentSpecs[attachmentIndex];
  glClearTexImage(m_ColorAttachments[attachmentIndex], 0,
                  Utils::toGLTextureFormat(spec.m_Format), GL_INT, &value);
}

void GLFramebuffer::bind() const {
  glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferId);
  glViewport(0, 0, m_Props.width, m_Props.height);

  /*int32_t value = -1;
  glClearTexImage(m_ColorAttachments[1], 0, GL_RED_INTEGER, GL_INT, &value);*/
}

void GLFramebuffer::unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

}  // namespace Hzn
