#pragma once

#include <glm/vec3.hpp>
#include <vector>

namespace world {

constexpr auto BLOCK_SIZE = 6;

//FIXME refactor that quickly
struct vertex_data
{
  vertex_data() = default;

  vertex_data(float vx, float vy, float vz, int nx, int ny, int nz)
    : vx(vx),
      vy(vy),
      vz(vz),
      nx(static_cast<float>(nx)),
      ny(static_cast<float>(ny)),
      nz(static_cast<float>(nz))
  {
  }

  float vx;
  float vy;
  float vz;
  float nx;
  float ny;
  float nz;
};

enum class BlockType {
  AIR,
  DIRT
};

struct Block
{
  BlockType type;
  glm::vec3 position;
};

[[nodiscard]] bool is_visible_block_type(BlockType type) noexcept;

}// namespace world