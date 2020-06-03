#include "Atlas.h"

textures::Atlas::Atlas(const std::string &texture_filename, std::size_t size, std::size_t texture_size)
  : Texture(texture_filename), _unit_size(1.0f / static_cast<float>(size / texture_size))
{
}

textures::TextureBoundaries textures::Atlas::get_texture_boundaries(const glm::vec2 &atlas_coordinate) const noexcept
{
  const glm::vec2 min{ atlas_coordinate.x * _unit_size, atlas_coordinate.y * _unit_size };
  const glm::vec2 max{ min.x + _unit_size, min.y + _unit_size };

  return {
    min,
    max
  };
}
