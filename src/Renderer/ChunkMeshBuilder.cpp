#include "ChunkMeshBuilder.h"
#include <Renderer/BlockFace.h>

[[nodiscard]] std::vector<renderer::Vertex> get_block_vertex_data(const world::Chunk &chunk, const world::Block &block);

renderer::Mesh renderer::ChunkMeshBuilder::get_mesh(const world::Chunk &chunk) noexcept
{
  renderer::Mesh mesh{};

  for (const auto &block : chunk.blocks) {
    if (!world::is_visible_block_type(block.type)) {
      continue;
    }

    if (!(block.position.x - 1 < 0
          || block.position.y - 1 < 0
          || block.position.z - 1 < 0
          || block.position.x + 1 >= world::CHUNK_SIZE_X
          || block.position.y + 1 >= world::CHUNK_SIZE_Y
          || block.position.z + 1 >= world::CHUNK_SIZE_Z)) {

      const auto &neighbour1 = chunk.blocks.at(static_cast<std::size_t>((block.position.x - 1) + world::CHUNK_SIZE_X * (block.position.y + world::CHUNK_SIZE_Y * block.position.z)));
      const auto &neighbour2 = chunk.blocks.at(static_cast<std::size_t>((block.position.x + 1) + world::CHUNK_SIZE_X * (block.position.y + world::CHUNK_SIZE_Y * block.position.z)));
      const auto &neighbour3 = chunk.blocks.at(static_cast<std::size_t>(block.position.x + world::CHUNK_SIZE_X * (block.position.y - 1 + world::CHUNK_SIZE_Y * block.position.z)));
      const auto &neighbour4 = chunk.blocks.at(static_cast<std::size_t>(block.position.x + world::CHUNK_SIZE_X * (block.position.y + 1 + world::CHUNK_SIZE_Y * block.position.z)));
      const auto &neighbour5 = chunk.blocks.at(static_cast<std::size_t>(block.position.x + world::CHUNK_SIZE_X * (block.position.y + world::CHUNK_SIZE_Y * (block.position.z - 1))));
      const auto &neighbour6 = chunk.blocks.at(static_cast<std::size_t>(block.position.x + world::CHUNK_SIZE_X * (block.position.y + world::CHUNK_SIZE_Y * (block.position.z + 1))));

      if (neighbour1.type == world::BlockType::DIRT
          && neighbour2.type == world::BlockType::DIRT
          && neighbour3.type == world::BlockType::DIRT
          && neighbour4.type == world::BlockType::DIRT
          && neighbour5.type == world::BlockType::DIRT
          && neighbour6.type == world::BlockType::DIRT) {
        continue;
      }
    }

    mesh.insert_vertex_data(get_block_vertex_data(chunk, block));
  }

  return mesh;
}

std::vector<renderer::Vertex> get_block_vertex_data(const world::Chunk &chunk, const world::Block &block)
{
  std::vector<renderer::Vertex> block_vertex_data{};

  block_vertex_data.reserve(36);

  const auto block_position = glm::vec3{
    chunk.position.x * world::CHUNK_SIZE_X + block.position.x,
    0 * world::CHUNK_SIZE_Y + block.position.y,
    chunk.position.y * world::CHUNK_SIZE_Z + block.position.z
  };

  for (const auto &face : renderer::BLOCK_MESH_FACES) {
    for (const auto &vertices : face.vertices) {
      block_vertex_data.push_back({ { block_position + vertices }, face.normal });
    }
  }

  return block_vertex_data;
}