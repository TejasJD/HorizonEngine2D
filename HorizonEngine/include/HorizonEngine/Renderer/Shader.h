#pragma once

#ifndef HZN_SHADER_H
#define HZN_SHADER_H

#include <glm/glm.hpp>

#include <cstdint>
#include <memory>
#include <initializer_list>
#include <utility>
#include <string>

namespace Hzn {
enum class ShaderType : uint8_t { None = 0, VertexShader, FragmentShader };

class Shader {
 public:
  virtual ~Shader() = default;

  //! calling this member function uses the shader.
  virtual void bind() const = 0;
  virtual void unbind() const = 0;
  virtual unsigned int getId() const = 0;
  virtual void setUniform(const std::string& s, const glm::mat4& mat4) = 0;
  virtual void setUniform(const std::string& s, int* a, uint32_t count) = 0;
  virtual void setUniform(const std::string& s, const glm::mat3& mat3) = 0;
  virtual void setUniform(const std::string& s, const glm::vec4& vec4) = 0;
  virtual void setUniform(const std::string& s, const glm::vec3& vec3) = 0;
  virtual void setUniform(const std::string& s, const glm::vec2& vec2) = 0;
  virtual void setUniform(const std::string& s, int ui) = 0;
  virtual void setUniform(const std::string& s, float uf) = 0;

  //! Method to retrieve GLShader API and initialise
  static std::shared_ptr<Shader> create(
      const std::initializer_list<std::pair<ShaderType, std::string>>&
          nameAndPath);
  //! Method to retrieve GLShader API and initialise with vertex shader and
  //! fragment shader
  static std::shared_ptr<Shader> create(const std::string& vertexSource,
                                        const std::string& fragmentSource);
};
}  // namespace Hzn

#endif  // !HZN_SHADER_H