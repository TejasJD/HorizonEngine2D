#pragma once

#ifndef HZN_GL_BUFFER_H
#define HZN_GL_BUFFER_H

#include "HorizonEngine/Renderer/Buffer.h"

namespace Hzn {

class GLVertexBuffer : public VertexBuffer {
 public:
  GLVertexBuffer(uint32_t size);
  GLVertexBuffer(float* vertices, uint32_t size);
  virtual ~GLVertexBuffer();

  virtual void bind() const override;
  virtual void unbind() const override;
  virtual void setBufferLayout(const BufferLayout& layout) override {
    m_Layout = layout;
  }
  virtual BufferLayout getBufferLayout() const override { return m_Layout; }
  virtual void setData(void* data, uint32_t size) override;

 private:
  uint32_t m_VertexBufferId = 0;
  BufferLayout m_Layout;
};

class GLElementBuffer : public ElementBuffer {
 public:
  GLElementBuffer(uint32_t* indices, uint32_t count);
  virtual ~GLElementBuffer();

  virtual void bind() const override;
  virtual void unbind() const override;

  virtual unsigned int size() const override { return m_Count; }

 private:
  uint32_t m_Count = 0;
  uint32_t m_ElementBufferId = 0;
};
}  // namespace Hzn

#endif  // !HZN_GL_BUFFER_H
