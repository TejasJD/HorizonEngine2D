#pragma once

#ifndef HZN_RENDER_CONTEXT_H
#define HZN_RENDER_CONTEXT_H

#include <memory>

namespace Hzn {
class RenderContext {
 public:
  virtual ~RenderContext() = default;

  virtual void init() = 0;
  //! Swap the buffers, one shown on screen, one to draw on
  virtual void swapBuffers() = 0;

  //! Used to retrieve the OpenGl or Graphics libraries available
  static std::shared_ptr<RenderContext> create(void* const& windowHandle);
};
}  // namespace Hzn

#endif  // !HZN_RENDER_CONTEXT_H
