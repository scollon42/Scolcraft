#pragma once

#include <glm/vec3.hpp>
#include <World/Blocks/BlockType.h>

namespace world::chunks {
struct ChunkBlock
{
  blocks::BlockType type;
  glm::vec3         position;
};

constexpr ChunkBlock DEFAULT_BLOCK{ blocks::BlockType::NONE, glm::vec3{ 0.0f } };

}// namespace world::chunks