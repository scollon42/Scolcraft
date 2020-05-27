#include "ChunkMeshBuilder.h"
#include <Renderer/BlockFace.h>
#include <spdlog/spdlog.h>

struct BlockTextureFaces
{
  std::array<glm::vec2, 6> sides;
  std::array<glm::vec2, 6>
    top;
  std::array<glm::vec2, 6> bottom;
};

[[nodiscard]] std::vector<renderer::Vertex>
  build_block_mesh_data(const world::Chunk &chunk, const world::Block &block, const BlockTextureFaces &block_texture_faces);

renderer::ChunkMeshBuilder::ChunkMeshBuilder(const textures::Atlas &atlas)
  : _atlas_texture(atlas)
{
}

renderer::Mesh renderer::ChunkMeshBuilder::get_mesh(const world::Chunk &chunk) const noexcept
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
      const BlockTextureFaces block_texture_faces{
        _atlas_texture.get_texture_coordinates({ 2, 0 }),
        _atlas_texture.get_texture_coordinates({ 0, 0 }),
        _atlas_texture.get_texture_coordinates({ 2, 0 })
      };

      const auto data = build_block_mesh_data(chunk, block, block_texture_faces);
      mesh.insert_vertex_data(data);
    }
  }

  return mesh;
}

std::vector<renderer::Vertex> build_block_mesh_data(const world::Chunk &chunk, const world::Block &block, const BlockTextureFaces &block_texture_faces)
{
  std::vector<renderer::Vertex> block_vertex_data{};

  block_vertex_data.reserve(36);

  const glm::vec3 block_position{ world::to_absolute_position(chunk, block.position) };

  for (const auto &face : renderer::BLOCK_MESH_FACES) {
    std::size_t i{ 0 };
    auto        texture_coordinates = block_texture_faces.sides;

    if (face.direction == renderer::FaceDirection::TOP) {
      texture_coordinates = block_texture_faces.top;
    } else if (face.direction == renderer::FaceDirection::BOTTOM) {
      texture_coordinates = block_texture_faces.bottom;
    }

    for (const auto &vertices : face.vertices) {
      const auto vertex = renderer::Vertex{ { block_position + vertices }, face.normal, texture_coordinates[i++] };
      block_vertex_data.push_back(vertex);
    }
  }

  return block_vertex_data;
}