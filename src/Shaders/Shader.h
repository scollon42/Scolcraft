#pragma once

#include <string>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace shaders {
class Shader
{
public:
  ~Shader();
  void bind() const noexcept;
  void unbind() const noexcept;

  [[nodiscard]] unsigned int get_id() const noexcept;

  void set_uniform(const std::string &name, const glm::vec3 &v) const noexcept;
  void set_uniform(const std::string &name, const glm::mat4 &v) const noexcept;

private:
  unsigned int _id;
};

}// namespace shaders
