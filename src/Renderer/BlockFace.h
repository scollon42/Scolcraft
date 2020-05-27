#pragma once

#include <array>
#include <Renderer/Vertex.h>
#include <glm/vec3.hpp>
namespace renderer {

enum class FaceDirection {
  LEFT,
  RIGHT,
  BOTTOM,
  TOP,
  FRONT,
  BACK
};

struct BlockFace
{
  FaceDirection            direction;
  std::array<glm::vec3, 6> vertices;
  glm::vec3                normal;
};

constexpr BlockFace LEFT{
  FaceDirection::LEFT,
  { glm::vec3{ 0.0f, 0.0f, 0.0f },
    glm::vec3{ 0.0f, 0.0f, 1.0f },
    glm::vec3{ 0.0f, 1.0f, 0.0f },
    glm::vec3{ 0.0f, 1.0f, 0.0f },
    glm::vec3{ 0.0f, 1.0f, 1.0f },
    glm::vec3{ 0.0f, 0.0f, 1.0f } },
  glm::vec3{ -1.0f, 0.0f, 0.0f }
};

constexpr BlockFace RIGHT{
  FaceDirection::RIGHT,
  { glm::vec3{ 1.0f, 0.0f, 0.0f },
    glm::vec3{ 1.0f, 0.0f, 1.0f },
    glm::vec3{ 1.0f, 1.0f, 0.0f },
    glm::vec3{ 1.0f, 1.0f, 0.0f },
    glm::vec3{ 1.0f, 1.0f, 1.0f },
    glm::vec3{ 1.0f, 0.0f, 1.0f } },
  glm::vec3{ 1.0f, 0.0f, 0.0f }
};

constexpr BlockFace BOTTOM{
  FaceDirection::BOTTOM,
  { glm::vec3{ 0.0f, 0.0f, 0.0f },
    glm::vec3{ 0.0f, 0.0f, 1.0f },
    glm::vec3{ 1.0f, 0.0f, 0.0f },
    glm::vec3{ 1.0f, 0.0f, 0.0f },
    glm::vec3{ 1.0f, 0.0f, 1.0f },
    glm::vec3{ 0.0f, 0.0f, 1.0f } },
  glm::vec3{ 0.0f, -1.0f, 0.0f }
};

constexpr BlockFace TOP{
  FaceDirection::TOP,
  { glm::vec3{ 0.0f, 1.0f, 0.0f },
    glm::vec3{ 0.0f, 1.0f, 1.0f },
    glm::vec3{ 1.0f, 1.0f, 0.0f },
    glm::vec3{ 1.0f, 1.0f, 0.0f },
    glm::vec3{ 1.0f, 1.0f, 1.0f },
    glm::vec3{ 0.0f, 1.0f, 1.0f } },
  glm::vec3{ 0.0f, 1.0f, 0.0f }
};

constexpr BlockFace FRONT{
  FaceDirection::FRONT,
  { glm::vec3{ 0.0f, 0.0f, 0.0f },
    glm::vec3{ 0.0f, 1.0f, 0.0f },
    glm::vec3{ 1.0f, 0.0f, 0.0f },
    glm::vec3{ 1.0f, 0.0f, 0.0f },
    glm::vec3{ 1.0f, 1.0f, 0.0f },
    glm::vec3{ 0.0f, 1.0f, 0.0f } },
  glm::vec3{ 0.0f, 0.0f, -1.0f }
};

constexpr BlockFace BACK{
  FaceDirection::BACK,
  { glm::vec3{ 0.0f, 0.0f, 1.0f },
    glm::vec3{ 1.0f, 0.0f, 1.0f },
    glm::vec3{ 0.0f, 1.0f, 1.0f },
    glm::vec3{ 0.0f, 1.0f, 1.0f },
    glm::vec3{ 1.0f, 1.0f, 1.0f },
    glm::vec3{ 1.0f, 0.0f, 1.0f } },
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
