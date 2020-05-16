#pragma once

#include <glm/vec3.hpp>
#include <vector>

namespace world {

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