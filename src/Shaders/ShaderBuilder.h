#pragma once

#include <string>
#include <vector>
#include <Shaders/Shader.h>

namespace shaders {
class ShaderBuilder final
{
public:
  [[nodiscard]] ShaderBuilder &      build() noexcept;
  [[nodiscard]] ShaderBuilder &      add_vertex(const std::string &vertex_src);
  [[nodiscard]] ShaderBuilder &      add_fragment(const std::string &fragment_str);
  [[nodiscard]] const ShaderBuilder &link();
  [[nodiscard]] shaders::Shader      get() const noexcept;

private:
  std::vector<unsigned int> _attached_shader_ids;
  shaders::Shader           _shader;
};
}// namespace shaders
