#pragma once

#include "glew_includes.h"
#include <map>

#include "Mesh.h"

namespace renderer {
class ChunkRenderer final
{
public:
  ChunkRenderer();
  ~ChunkRenderer();
  void                  update_mesh(int chunk_id, const Mesh &mesh) noexcept;
  [[maybe_unused]] void delete_mesh(int chunk_id) noexcept;
  [[maybe_unused]] void clear_mesh() noexcept;


  void render() const noexcept;

private:
  std::map<int, Mesh>   _meshes;
  std::map<int, GLuint> _built_meshes_ids;
  GLuint                _vertex_array_id{};
};
}// namespace renderer
