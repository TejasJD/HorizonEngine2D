#include "pch.h"

#include "HorizonEngine/Renderer/RendererAPI.h"
#include "HorizonEngine/Renderer/Texture.h"

#include "OpenGL/GLTexture.h"

namespace Hzn {
/// <summary>
/// function that allows you to create texture.
/// If OpenGl is chosen, returns shared_ptr to the GLTextures class
/// </summary>
/// <param name="path"></param>
/// <returns></returns>
std::shared_ptr<Texture2D> Texture2D::create(const std::string& path) {
  switch (RendererAPI::getAPI()) {
    case RendererAPI::API::None:
      HZN_CORE_ASSERT(false, "No Render API Selected");
      return nullptr;
    case RendererAPI::API::OpenGL:
      return std::make_shared<GLTexture2D>(path);
  }

  HZN_CORE_ASSERT(false, "Invalid API selected!");
  return nullptr;
}

/// <summary>
/// function that allows you to create texture.
/// If OpenGl is chosen, returns shared_ptr to the GLTextures class.
/// Takes in a width & Height
/// </summary>
/// <param name="width"></param>
/// <param name="height"></param>
/// <returns></returns>
std::shared_ptr<Hzn::Texture2D> Texture2D::create(uint32_t width,
                                                  uint32_t height) {
  switch (RendererAPI::getAPI()) {
    case RendererAPI::API::None:
      HZN_CORE_ASSERT(false, "No Render API Selected");
      return nullptr;
    case RendererAPI::API::OpenGL:
      return std::make_shared<GLTexture2D>(width, height);
  }

  HZN_CORE_ASSERT(false, "Invalid API selected!");
  return nullptr;
}

}  // namespace Hzn