#include "Atlas.h"

textures::Atlas::Atlas(const std::string &texture_filename, std::size_t size, std::size_t texture_size)
  : Texture(texture_filename), _size(size), _texture_size(texture_size)
{
}

std::array<glm::vec2, 6> textures::Atlas::get_texture_coordinates(const glm::vec2 &coordinate) const noexcept
{
  const static auto textures_per_row = _size / _texture_size;
  const static auto unit_size        = 1.0f / static_cast<float>(textures_per_row);

  auto x_min = coordinate.x * unit_size;
  auto y_min = coordinate.y * unit_size;

  auto x_max = x_min + unit_size;
  auto y_max = y_min + unit_size;

  return {
    glm::vec2{ x_min, y_min },
    glm::vec2{ x_min, y_max },
    glm::vec2{ x_max, y_min },

    glm::vec2{ x_max, y_min },
    glm::vec2{ x_max, y_max },
    glm::vec2{ x_min, y_max },
  };
}
