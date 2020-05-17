#pragma once

#include <array>
#include <Renderer/Vertex.h>
#include <glm/vec3.hpp>
namespace renderer {

struct BlockFace
{
  std::array<glm::vec3, 6> vertices;
  glm::vec3                normal;
};

constexpr BlockFace LEFT{
  { glm::vec3{ 0.0f, 0.0f, 0.0f },
    glm::vec3{ 0.0f, 0.0f, 1.0f },
    glm::vec3{ 0.0f, 1.0f, 0.0f },
    glm::vec3{ 0.0f, 1.0f, 0.0f },
    glm::vec3{ 0.0f, 0.0f, 1.0f },
    glm::vec3{ 0.0f, 1.0f, 1.0f } },
  glm::vec3{ -1.0f, 0.0f, 0.0f }
};

constexpr BlockFace RIGHT{
  { glm::vec3{ 1.0f, 0.0f, 0.0f },
    glm::vec3{ 1.0f, 1.0f, 0.0f },
    glm::vec3{ 1.0f, 0.0f, 1.0f },
    glm::vec3{ 1.0f, 1.0f, 0.0f },
    glm::vec3{ 1.0f, 1.0f, 1.0f },
    glm::vec3{ 1.0f, 0.0f, 1.0f } },
  glm::vec3{ 1.0f, 0.0f, 0.0f }
};

constexpr BlockFace BOTTOM{
  { glm::vec3{ 0.0f, 0.0f, 0.0f },
    glm::vec3{ 1.0f, 0.0f, 0.0f },
    glm::vec3{ 0.0f, 0.0f, 1.0f },
    glm::vec3{ 1.0f, 0.0f, 0.0f },
    glm::vec3{ 1.0f, 0.0f, 1.0f },
    glm::vec3{ 0.0f, 0.0f, 1.0f } },
  glm::vec3{ 0.0f, -1.0f, 0.0f }
};

constexpr BlockFace TOP{
  { glm::vec3{ 0.0f, 1.0f, 0.0f },
    glm::vec3{ 0.0f, 1.0f, 1.0f },
    glm::vec3{ 1.0f, 1.0f, 0.0f },
    glm::vec3{ 1.0f, 1.0f, 0.0f },
    glm::vec3{ 0.0f, 1.0f, 1.0f },
    glm::vec3{ 1.0f, 1.0f, 1.0f } },
  glm::vec3{ 0.0f, 1.0f, 0.0f }
};

constexpr BlockFace BACK{
  { glm::vec3{ 0.0f, 0.0f, 0.0f },
    glm::vec3{ 0.0f, 1.0f, 0.0f },
    glm::vec3{ 1.0f, 0.0f, 0.0f },
    glm::vec3{ 0.0f, 1.0f, 0.0f },
    glm::vec3{ 1.0f, 1.0f, 0.0f },
    glm::vec3{ 1.0f, 0.0f, 0.0f } },
  glm::vec3{ 0.0f, 0.0f, -1.0f }
};

constexpr BlockFace FRONT{
  { glm::vec3{ 0.0f, 0.0f, 1.0f },
    glm::vec3{ 1.0f, 0.0f, 1.0f },
    glm::vec3{ 0.0f, 1.0f, 1.0f },
    glm::vec3{ 0.0f, 1.0f, 1.0f },
    glm::vec3{ 1.0f, 0.0f, 1.0f },
    glm::vec3{ 1.0f, 1.0f, 1.0f } },
  glm::vec3{ 0.0f, 0.0f, 1.0f }
};

constexpr std::array<BlockFace, 6> BLOCK_MESH_FACES{
  LEFT,
  RIGHT,
  FRONT,
  BACK,
  TOP,
  BOTTOM
};

}// namespace renderer
