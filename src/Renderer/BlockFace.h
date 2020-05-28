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

struct BlockData
{
  glm::vec3 vertex;
  glm::vec2 texture;
};

struct BlockFace
{
  FaceDirection            direction;
  std::array<BlockData, 6> face_data;
  glm::vec3                normal;
};

constexpr BlockFace LEFT{
  FaceDirection::LEFT,
  { BlockData{ glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec2{ 1, 1 } },
    BlockData{ glm::vec3{ 0.0f, 0.0f, 1.0f }, glm::vec2{ 0, 1 } },
    BlockData{ glm::vec3{ 0.0f, 1.0f, 1.0f }, glm::vec2{ 0, 0 } },
    BlockData{ glm::vec3{ 0.0f, 1.0f, 1.0f }, glm::vec2{ 0, 0 } },
    BlockData{ glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::vec2{ 1, 0 } },
    BlockData{ glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec2{ 1, 1 } } },
  glm::vec3{ -1.0f, 0.0f, 0.0f }
};// namespace renderer

constexpr BlockFace RIGHT{
  FaceDirection::RIGHT,
  { BlockData{ glm::vec3{ 1.0f, 0.0f, 0.0f }, glm::vec2{ 1, 1 } },
    BlockData{ glm::vec3{ 1.0f, 0.0f, 1.0f }, glm::vec2{ 0, 1 } },
    BlockData{ glm::vec3{ 1.0f, 1.0f, 1.0f }, glm::vec2{ 0, 0 } },
    BlockData{ glm::vec3{ 1.0f, 1.0f, 1.0f }, glm::vec2{ 0, 0 } },
    BlockData{ glm::vec3{ 1.0f, 1.0f, 0.0f }, glm::vec2{ 1, 0 } },
    BlockData{ glm::vec3{ 1.0f, 0.0f, 0.0f }, glm::vec2{ 1, 1 } } },
  glm::vec3{ 1.0f, 0.0f, 0.0f }
};

constexpr BlockFace BOTTOM{
  FaceDirection::BOTTOM,
  { BlockData{ glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec2{ 1, 1 } },
    BlockData{ glm::vec3{ 1.0f, 0.0f, 0.0f }, glm::vec2{ 0, 1 } },
    BlockData{ glm::vec3{ 1.0f, 0.0f, 1.0f }, glm::vec2{ 0, 0 } },
    BlockData{ glm::vec3{ 1.0f, 0.0f, 1.0f }, glm::vec2{ 0, 0 } },
    BlockData{ glm::vec3{ 0.0f, 0.0f, 1.0f }, glm::vec2{ 1, 0 } },
    BlockData{ glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec2{ 1, 1 } } },
  glm::vec3{ 0.0f, -1.0f, 0.0f }
};

constexpr BlockFace TOP{
  FaceDirection::TOP,
  { BlockData{ glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::vec2{ 1, 1 } },
    BlockData{ glm::vec3{ 1.0f, 1.0f, 0.0f }, glm::vec2{ 0, 1 } },
    BlockData{ glm::vec3{ 1.0f, 1.0f, 1.0f }, glm::vec2{ 0, 0 } },
    BlockData{ glm::vec3{ 1.0f, 1.0f, 1.0f }, glm::vec2{ 0, 0 } },
    BlockData{ glm::vec3{ 0.0f, 1.0f, 1.0f }, glm::vec2{ 1, 0 } },
    BlockData{ glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::vec2{ 1, 1 } } },
  glm::vec3{ 0.0f, 1.0f, 0.0f }
};

constexpr BlockFace FRONT{
  FaceDirection::FRONT,
  { BlockData{ glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec2{ 1, 1 } },
    BlockData{ glm::vec3{ 1.0f, 0.0f, 0.0f }, glm::vec2{ 0, 1 } },
    BlockData{ glm::vec3{ 1.0f, 1.0f, 0.0f }, glm::vec2{ 0, 0 } },
    BlockData{ glm::vec3{ 1.0f, 1.0f, 0.0f }, glm::vec2{ 0, 0 } },
    BlockData{ glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::vec2{ 1, 0 } },
    BlockData{ glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec2{ 1, 1 } } },
  glm::vec3{ 0.0f, 0.0f, -1.0f }
};

constexpr BlockFace BACK{
  FaceDirection::BACK,
  { BlockData{ glm::vec3{ 0.0f, 0.0f, 1.0f }, glm::vec2{ 1, 1 } },
    BlockData{ glm::vec3{ 1.0f, 0.0f, 1.0f }, glm::vec2{ 0, 1 } },
    BlockData{ glm::vec3{ 1.0f, 1.0f, 1.0f }, glm::vec2{ 0, 0 } },
    BlockData{ glm::vec3{ 1.0f, 1.0f, 1.0f }, glm::vec2{ 0, 0 } },
    BlockData{ glm::vec3{ 0.0f, 1.0f, 1.0f }, glm::vec2{ 1, 0 } },
    BlockData{ glm::vec3{ 0.0f, 0.0f, 1.0f }, glm::vec2{ 1, 1 } } },
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
