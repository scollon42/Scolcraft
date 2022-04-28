#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

#include "ChunkBlock.h"
#include "Renderer/Mesh.h"

namespace world::chunks {

constexpr auto CHUNK_SIZE_X = 16;
constexpr auto CHUNK_SIZE_Y = 16;
constexpr auto CHUNK_SIZE_Z = 16;

struct Chunk
{
  Chunk(int id, const glm::vec2 &position);

  int                     id;
  glm::vec2               position;
  std::vector<ChunkBlock> blocks;
};

[[nodiscard]] inline glm::vec3 to_absolute_position(const Chunk &chunk, glm::vec3 position) noexcept
{
  return glm::vec3{
    chunk.position.x * CHUNK_SIZE_X + position.x,
    position.y,
    chunk.position.y * CHUNK_SIZE_Z + position.z
  };
}

[[nodiscard]] inline glm::vec3 to_relative_position(const Chunk &chunk, glm::vec3 position) noexcept
{
  return glm::vec3{
    position.x - chunk.position.x - CHUNK_SIZE_X,
    position.y - CHUNK_SIZE_Y,
    position.z - chunk.position.y - CHUNK_SIZE_Z
  };
}
[[nodiscard]] inline int get_index_at(const glm::vec3 &position)
{
  const auto index = position.z * CHUNK_SIZE_Y * CHUNK_SIZE_X + position.y * CHUNK_SIZE_X + position.x;
  return static_cast<int>(index);
}

[[nodiscard]] std::vector<ChunkBlock> get_neighbours_blocks(const Chunk &chunk, const glm::vec3 &position) noexcept;

[[maybe_unused]] [[nodiscard]] const ChunkBlock &get_absolute_block_at(const Chunk &chunk, const glm::vec3 &position);

[[nodiscard]] const ChunkBlock &get_relative_block_at(const Chunk &chunk, const glm::vec3 &position);

[[maybe_unused]] void update_block_type_at(Chunk &chunk, const glm::vec3 &position, blocks::BlockType blockType) noexcept;

}// namespace world::chunks
