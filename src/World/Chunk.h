#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>

#include "Block.h"
#include "Renderer/Mesh.h"

namespace world {

constexpr auto CHUNK_SIZE_X = 16;
constexpr auto CHUNK_SIZE_Y = 256;
constexpr auto CHUNK_SIZE_Z = 16;

struct Chunk
{
  Chunk(unsigned int id, const glm::vec3 &position);

  unsigned int id;
  glm::vec3 position;
  std::vector<Block> blocks;
  //  renderer::Mesh mesh;
};

[[nodiscard]] world::Chunk generate_chunk(unsigned int id, const glm::vec3 &position);

[[nodiscard]] glm::vec3 absolute_block_position(const Chunk &chunk, const Block &block) noexcept;
[[nodiscard]] glm::vec3 relative_block_position(const world::Chunk &chunk, const world::Block &block) noexcept;

[[nodiscard]] bool is_next_to_air_block(const Chunk &chunk, const Block &block) noexcept;

[[nodiscard]] const Block &absolute_block_at(const Chunk &chunk, const glm::vec3 &position);

[[nodiscard]] renderer::Mesh get_chunk_mesh(const world::Chunk &chunk) noexcept;

[[nodiscard]] std::vector<renderer::Vertex> get_block_vertex_data(const world::Chunk &chunk, const world::Block &block);

}// namespace world