#pragma once

#include <GL/glew.h>
#include <map>

#include "Mesh.h"

namespace renderer {
class ChunkRenderer final
{
public:
  ChunkRenderer();
  ~ChunkRenderer();
  void update_mesh(unsigned int chunk_id, const Mesh &mesh) noexcept;
  void delete_mesh(unsigned int chunk_id) noexcept;
  void render() const noexcept;

private:
  std::map<unsigned int, Mesh> _meshes;
  std::map<unsigned int, GLuint> _built_meshes_ids;
  GLuint _vertex_array_id{};
};
}// namespace renderer
