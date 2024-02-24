#include "pch.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "GLBuffer.h"

namespace Hzn {
//! ---------- GL VERTEX BUFFER MEMBER FUNCTION DEFINITIONS ----------

GLVertexBuffer::GLVertexBuffer(float* vertices, uint32_t size) {
  glGenBuffers(1, &m_VertexBufferId);
  glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferId);
  glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

GLVertexBuffer::GLVertexBuffer(uint32_t size) {
  glGenBuffers(1, &m_VertexBufferId);
  glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferId);
  glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

GLVertexBuffer::~GLVertexBuffer() { glDeleteBuffers(1, &m_VertexBufferId); }

void GLVertexBuffer::bind() const {
  glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferId);
}

void GLVertexBuffer::unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void GLVertexBuffer::setData(void* data, uint32_t size) {
  bind();
  glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

//! ---------- GL ELEMENT BUFFER MEMBER FUNCTION DEFINITIONS ----------

GLElementBuffer::GLElementBuffer(uint32_t* indices, uint32_t count)
    : m_Count(count) {
  glGenBuffers(1, &m_ElementBufferId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBufferId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(int), indices,
               GL_STATIC_DRAW);
}

GLElementBuffer::~GLElementBuffer() { glDeleteBuffers(1, &m_ElementBufferId); }

void GLElementBuffer::bind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBufferId);
}

void GLElementBuffer::unbind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
}  // namespace Hzn