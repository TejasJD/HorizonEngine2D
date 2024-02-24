#pragma once

#ifndef HZN_RENDERER_API_H
#define HZN_RENDERER_API_H

#include <glm/glm.hpp>

#include "HorizonEngine/Renderer/VertexArray.h"

namespace Hzn {
class RendererAPI {
 public:
  virtual ~RendererAPI() = default;

  //! Either no API or OpenGL for now, scope to add more
  enum class API { None = 0, OpenGL = 1 };

  //! Returns current API (will always be OpenGL for now)
  inline static RendererAPI::API getAPI() { return m_Api; }

  //! calls init from RenderCall.cpp
  virtual void init() = 0;
  virtual void setClearColor(const glm::vec4& color) = 0;
  virtual void submitClear() = 0;
  virtual void drawElements(const std::shared_ptr<VertexArray>& vertexArray,
                            uint32_t count = 0) = 0;
  virtual void setViewport(unsigned int x, unsigned int y, unsigned int width,
                           unsigned int height) = 0;

 private:
  static API m_Api;
};

}  // namespace Hzn
#endif  // !HZN_RENDERER_API
