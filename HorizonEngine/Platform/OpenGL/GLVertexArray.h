#pragma once

#ifndef HZN_GL_VERTEX_ARRAY_H
#define HZN_GL_VERTEX_ARRAY_H

#include "HorizonEngine/Renderer/VertexArray.h"

namespace Hzn {
class GLVertexArray : public VertexArray {
 public:
  GLVertexArray();
  virtual ~GLVertexArray();

  // Inherited via VertexArray
  virtual void bind() const override;
  virtual void unbind() const override;
  virtual void addVertexBuffer(
      const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
  virtual void setElementBuffer(
      const std::shared_ptr<ElementBuffer>& elementBuffer) override;

  virtual std::shared_ptr<ElementBuffer> getElementBuffer() override {
    return m_ElementBuffer;
  }
  virtual std::vector<std::shared_ptr<VertexBuffer>> getVertexBuffers()
      override {
    return m_VertexBuffers;
  }

 private:
  unsigned int m_VertexArrayId = 0;
  std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
  std::shared_ptr<ElementBuffer> m_ElementBuffer;
};
}  // namespace Hzn

#endif  // !HZN_GL_VERTEX_ARRAY_H
