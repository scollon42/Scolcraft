#pragma once

#include <Renderer/Mesh.h>
#include <Renderer/Vertex.h>
#include <World/Chunks/Chunk.h>
#include <World/Blocks/Block.h>
#include <Texture/Atlas.h>

namespace renderer {
class ChunkMeshBuilder
{
public:
  explicit ChunkMeshBuilder(const textures::Atlas &atlas);

  [[nodiscard]] Mesh get_mesh(const std::unordered_map<world::blocks::BlockType, world::blocks::Block> &block_data, const world::chunks::Chunk &chunk) const noexcept;

private:
  const textures::Atlas &_atlas_texture;

};// namespace ChunkMeshBuilder
}// namespace renderer
