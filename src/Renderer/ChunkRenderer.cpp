#include "ChunkRenderer.h"

[[maybe_unused]] void renderer::ChunkRenderer::remove_chunk(unsigned int chunk_id) noexcept
{
  this->_chunk_meshes.erase(chunk_id);
}

void renderer::ChunkRenderer::update_mesh(unsigned int chunk_id, const Mesh &mesh) noexcept
{
  this->_chunk_meshes.emplace(chunk_id, mesh);
}

void renderer::ChunkRenderer::render() const noexcept
{
  for (const auto &mesh : this->_chunk_meshes) {
    mesh.second.render();
  }
}
