#pragma once

#include <glm/vec3.hpp>
#include <vector>

namespace world {

enum class BlockType {
  NONE,
  AIR,
  DIRT
};

struct Block
{
  BlockType type;
  glm::vec3 position;
};

constexpr Block DEFAULT_BLOCK{ BlockType::NONE, glm::vec3{ 0.0f } };

[[nodiscard]] bool is_visible_block_type(BlockType type) noexcept;

}// namespace world