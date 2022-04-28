
#include "WorldRenderer.h"

renderer::WorldRenderer::WorldRenderer(const world::World &world, const textures::Atlas & atlas) :
  _chunk_mesh_builder(std::make_unique<renderer::ChunkMeshBuilder>(atlas, world.get_block_data())),
  _chunk_renderer(std::make_unique<renderer::ChunkRenderer>())
{
}

void renderer::WorldRenderer::update(const world::World &world, const Camera &camera) noexcept
{
  static auto last_index = INT_MIN;

  const auto index = world::get_index_at_player(camera.get_position());

  if (index == last_index) return;

  spdlog::info("Index changed from {} to {}.", last_index, index);
  last_index = index;

  _chunk_renderer->clear_mesh();

  for (const auto &chunk : world.get_chunks_around(camera.get_position(), 5)) {
    const auto &mesh = _chunk_mesh_builder->get_mesh(chunk);
    _chunk_renderer->update_mesh(chunk.id, mesh);
  }
}

void renderer::WorldRenderer::render() const noexcept
{
  _chunk_renderer->render();
}
