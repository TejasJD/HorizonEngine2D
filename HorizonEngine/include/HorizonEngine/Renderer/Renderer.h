#pragma once

#ifndef HZN_RENDERER_H
#define HZN_RENDERER_H

#include "HorizonEngine/Renderer/VertexArray.h"
#include "HorizonEngine/Renderer/Buffer.h"
#include "HorizonEngine/Renderer/Shader.h"
#include "HorizonEngine/Renderer/Texture.h"
#include "HorizonEngine/Renderer/RenderCall.h"
#include "HorizonEngine/Camera/Camera.h"

namespace Hzn {
class Renderer {
 public:
  //! Calss to init th renderer from the supplying graphics library (OpenGL)
  static void init();
  //! Resizes the window
  static void onWindowResize(unsigned int width, unsigned int height);
  //! retrieves the renderAPI
  inline static RendererAPI::API getAPI() { return RendererAPI::getAPI(); }
};
}  // namespace Hzn

#endif  // !HZN_RENDERER_H
