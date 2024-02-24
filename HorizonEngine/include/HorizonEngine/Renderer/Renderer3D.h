#pragma once

#ifndef HZN_RENDERER_3D_H
#define HZN_RENDERER_3D_H

#include "HorizonEngine/Camera/Camera.h"

namespace Hzn {
class Renderer3D {
 public:
  static void init();
  static void destroy();

  static void beginScene(const OrthographicCamera& camera);
  static void endScene();

 private:
};
}  // namespace Hzn

#endif