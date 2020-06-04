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
  ChunkMeshBuilder(
    const textures::Atlas &                                                   atlas,
    const std::unordered_map<world::blocks::BlockType, world::blocks::Block> &block_data);

  [[nodiscard]] Mesh get_mesh(const world::chunks::Chunk &chunk) const noexcept;

private:
  const textures::Atlas &                                                   _atlas_texture;
  const std::unordered_map<world::blocks::BlockType, world::blocks::Block> &_block_data;

};// namespace ChunkMeshBuilder
}// namespace renderer
