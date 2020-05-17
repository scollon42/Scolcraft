#pragma once

#include <Shaders/Shader.h>

namespace shaders {
class DefaultShader final : public Shader
{
public:
  [[nodiscard]] static DefaultShader create();
  void                               set_model(const glm::mat4 &model) const noexcept;
  void                               set_view(const glm::mat4 &view) const noexcept;
  void                               set_projection(const glm::mat4 &projection) const noexcept;
  void                               set_view_position(const glm::vec3 &position) const noexcept;
  void                               set_object_color(const glm::vec3 &color) const noexcept;
  void                               set_light_color(const glm::vec3 &color) const noexcept;
  void                               set_light_position(const glm::vec3 &position) const noexcept;

private:
  explicit DefaultShader(Shader &&shader);
};
}// namespace shaders
