#pragma once

#include <map>

#include "Shaders/DefaultProgram.h"
#include "Mesh.h"

namespace renderer {
class ChunkRenderer
{
public:
  void remove_chunk(unsigned int chunk_id) noexcept;
  void update_mesh(unsigned int chunk_id, const Mesh &meshes) noexcept;

  void render() noexcept;

private:
  std::map<unsigned int, Mesh> _chunk_meshes;
};
}// namespace renderer
