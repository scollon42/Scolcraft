#pragma once

#include "ChunkRenderer.h"
#include "ChunkMeshBuilder.h"
#include "World/World.h"
#include "Texture/Atlas.h"
#include "Camera.h"

namespace renderer {
class WorldRenderer
{
public:
  explicit WorldRenderer(const world::World &world, const textures::Atlas &atlas);

  void update(const world::World &world, const Camera &camera) noexcept;
  void render() const noexcept;

private:
  std::unique_ptr<renderer::ChunkMeshBuilder> _chunk_mesh_builder;
  std::unique_ptr<renderer::ChunkRenderer> _chunk_renderer;

};
}// namespace renderer
