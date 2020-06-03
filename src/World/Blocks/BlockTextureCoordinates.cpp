#include "BlockTextureCoordinates.h"

world::blocks::BlockTextureCoordinates::BlockTextureCoordinates(const glm::vec2 &all)
  : sides(all), top(all), bottom(all)
{
}
world::blocks::BlockTextureCoordinates::BlockTextureCoordinates(
  const glm::vec2 &sides,
  const glm::vec2 &top,
  const glm::vec2 &bottom)
  : sides(sides),
    top(top),
    bottom(bottom)
{
}
