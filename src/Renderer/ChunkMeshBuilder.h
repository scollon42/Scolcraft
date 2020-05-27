#pragma once

#include <Renderer/Mesh.h>
#include <Renderer/Vertex.h>
#include <World/Chunk.h>
#include <Texture/Atlas.h>

namespace renderer {
class ChunkMeshBuilder
{
public:
  explicit ChunkMeshBuilder(const textures::Atlas &atlas);

  [[nodiscard]] Mesh get_mesh(const world::Chunk &chunk) const noexcept;

private:
  const textures::Atlas &_atlas_texture;
};// namespace ChunkMeshBuilder
}// namespace renderer
