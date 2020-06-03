#pragma once

#include <glm/vec2.hpp>

namespace world::blocks {
struct BlockTextureCoordinates
{
  BlockTextureCoordinates() = default;
  explicit BlockTextureCoordinates(const glm::vec2 &all);
  BlockTextureCoordinates(
    const glm::vec2 &sides,
    const glm::vec2 &top,
    const glm::vec2 &bottom);

  glm::vec2 sides;
  glm::vec2 top;
  glm::vec2 bottom;
};
}// namespace world::blocks
