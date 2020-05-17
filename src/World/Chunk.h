#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>

#include "Block.h"
#include "Renderer/Mesh.h"

namespace world {

constexpr auto CHUNK_SIZE_X = 16;
constexpr auto CHUNK_SIZE_Y = 16;
constexpr auto CHUNK_SIZE_Z = 16;

struct Chunk
{
  Chunk(int id, const glm::vec2 &position);

  int id;
  glm::vec2 position;
  std::vector<Block> blocks;
  //  renderer::Mesh mesh;
};

[[nodiscard]] world::Chunk generate_chunk(int id, const glm::vec2 &position);

[[nodiscard]] std::vector<Block> get_neighbours_blocks(const Chunk &chunk, const glm::vec3 &position) noexcept;

[[nodiscard]] const Block &get_absolute_block_at(const Chunk &chunk, const glm::vec3 &position);

[[nodiscard]] const Block &get_relative_block_at(const Chunk &chunk, const glm::vec3 &position);

[[nodiscard]] inline glm::vec3 to_absolute_position(const Chunk &chunk, glm::vec3 position) noexcept
{
  return glm::vec3{
    chunk.position.x * CHUNK_SIZE_X + position.x,
    position.y,
    chunk.position.y * CHUNK_SIZE_Z + position.z
  };
}
[[nodiscard]] inline glm::vec3 to_relative_position(const world::Chunk &chunk, glm::vec3 position) noexcept
{
  return glm::vec3{
    position.x - chunk.position.x - CHUNK_SIZE_X,
    position.y - CHUNK_SIZE_Y,
    position.z - chunk.position.y - CHUNK_SIZE_Z
  };
}

[[nodiscard]] inline int get_index_at(const glm::vec3 &position)
{
  return static_cast<int>(position.x + CHUNK_SIZE_X * (position.y + CHUNK_SIZE_Y * position.z));
}

}// namespace world