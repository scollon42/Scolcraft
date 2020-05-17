#include "ChunkMeshBuilder.h"
#include <Renderer/BlockFace.h>

[[nodiscard]] std::vector<renderer::Vertex> build_block_mesh_data(const world::Chunk &chunk, const world::Block &block);

renderer::Mesh renderer::ChunkMeshBuilder::get_mesh(const world::Chunk &chunk) noexcept
{
  renderer::Mesh mesh{};

  for (const auto &block : chunk.blocks) {
    if (!world::is_visible_block_type(block.type)) {
      continue;
    }

    const auto &neighbours = world::get_neighbours_blocks(chunk, block.position);

    bool should_be_drawn = false;
    for (const auto &neighbour : neighbours) {
      if (!world::is_visible_block_type(neighbour.type)) {
        should_be_drawn = true;
        break;
      }
    }

    if (should_be_drawn) {
      mesh.insert_vertex_data(build_block_mesh_data(chunk, block));
    }
  }

  return mesh;
}

std::vector<renderer::Vertex> build_block_mesh_data(const world::Chunk &chunk, const world::Block &block)
{
  std::vector<renderer::Vertex> block_vertex_data{};

  block_vertex_data.reserve(36);

  const glm::vec3 block_position{ world::to_absolute_position(chunk, block.position) };

  for (const auto &face : renderer::BLOCK_MESH_FACES) {
    for (const auto &vertices : face.vertices) {
      block_vertex_data.push_back({ { block_position + vertices }, face.normal });
    }
  }

  return block_vertex_data;
}