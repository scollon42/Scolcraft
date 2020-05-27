#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace renderer {
struct Vertex final
{
  glm::vec3 vertex;
  glm::vec3 normal;
  glm::vec2 texture;
};
}// namespace renderer
