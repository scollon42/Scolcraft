#pragma once

#include <glm/vec3.hpp>

namespace renderer {
struct Vertex final
{
  glm::vec3 vertex;
  glm::vec3 normal;
};
}// namespace renderer
