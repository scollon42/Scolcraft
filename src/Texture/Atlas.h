#pragma once

#include <Texture/Texture.h>
#include <memory>
#include <vector>
#include <glm/vec2.hpp>

namespace textures {

struct TextureBoundaries
{
  glm::vec2 min;
  glm::vec2 max;
};

class Atlas final : public Texture
{
public:
  Atlas(const std::string &texture_filename, std::size_t size, std::size_t texture_size);

  [[nodiscard]] TextureBoundaries get_texture_boundaries(const glm::vec2 &atlas_coordinate) const noexcept;

private:
  const float _unit_size;
};
}// namespace textures
