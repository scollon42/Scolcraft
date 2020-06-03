#include "ChunkMeshBuilder.h"
#include <Renderer/BlockFace.h>
#include <spdlog/spdlog.h>

using namespace world;

struct BlockTextureFaces
{
  textures::TextureBoundaries sides;
  textures::TextureBoundaries top;
  textures::TextureBoundaries bottom;
};

[[nodiscard]] std::vector<renderer::Vertex>
  build_block_mesh_data(const chunks::Chunk &chunk, const chunks::ChunkBlock &block, const BlockTextureFaces &block_texture_faces);

renderer::ChunkMeshBuilder::ChunkMeshBuilder(const textures::Atlas &atlas)
  : _atlas_texture(atlas)
{
}

inline bool should_be_drawn(const std::unordered_map<blocks::BlockType, blocks::Block> &block_data, const std::vector<chunks::ChunkBlock> &neighbours) noexcept
{
  bool should_be_drawn = false;

  for (const auto &neighbour : neighbours) {
    if (neighbour.type == blocks::BlockType::NONE) {
      should_be_drawn = true;
      break;
    }

    const auto &neighbour_data = block_data.find(neighbour.type);
    if (neighbour_data == block_data.end()) {
      should_be_drawn = true;
      break;

    } else {
      if (!neighbour_data->second.visible) {
        should_be_drawn = true;
        break;
      }
    }
  }
  return should_be_drawn;
}

renderer::Mesh renderer::ChunkMeshBuilder::get_mesh(const std::unordered_map<blocks::BlockType, blocks::Block> &block_data, const chunks::Chunk &chunk) const noexcept
{
  renderer::Mesh mesh{};

  for (const auto block : chunk.blocks) {
    const auto &data = block_data.find(block.type);

    if (data == block_data.end()) {
      spdlog::error("{} is not a valid block type", block.type);
      continue;
    }

    if (!data->second.visible) {
      continue;
    }

    const auto &neighbours = chunks::get_neighbours_blocks(chunk, block.position);


    if (should_be_drawn(block_data, neighbours)) {

      const BlockTextureFaces block_texture_faces{
        _atlas_texture.get_texture_boundaries(data->second.texture_coordinates.sides),
        _atlas_texture.get_texture_boundaries(data->second.texture_coordinates.top),
        _atlas_texture.get_texture_boundaries(data->second.texture_coordinates.bottom)
      };

      mesh.insert_vertex_data(build_block_mesh_data(chunk, block, block_texture_faces));
    }
  }

  return mesh;
}

inline glm::vec2 build_texture_coordinates(const textures::TextureBoundaries &boundaries, glm::vec2 base) noexcept
{
  return {
    base.x == 1 ? boundaries.max.x : boundaries.min.x,
    base.y == 1 ? boundaries.max.y : boundaries.min.y
  };
}

std::vector<renderer::Vertex> build_block_mesh_data(const chunks::Chunk &chunk, const chunks::ChunkBlock &block, const BlockTextureFaces &block_texture_faces)
{
  std::vector<renderer::Vertex> block_vertex_data{};

  block_vertex_data.reserve(36);

  const glm::vec3 block_position{ chunks::to_absolute_position(chunk, block.position) };

  for (const auto &face : renderer::BLOCK_MESH_FACES) {
    auto texture_coordinates = block_texture_faces.sides;

    if (face.direction == renderer::FaceDirection::TOP) {
      texture_coordinates = block_texture_faces.top;
    } else if (face.direction == renderer::FaceDirection::BOTTOM) {
      texture_coordinates = block_texture_faces.bottom;
    }

    for (const auto &data : face.face_data) {
      const auto vertex = renderer::Vertex{
        { block_position + data.vertex },
        face.normal,
        build_texture_coordinates(texture_coordinates, data.texture)
      };
      block_vertex_data.push_back(vertex);
    }
  }

  return block_vertex_data;
}