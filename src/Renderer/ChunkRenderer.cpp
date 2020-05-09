#include "ChunkRenderer.h"
#include <spdlog/spdlog.h>

void bind(GLuint vertex_array_id) noexcept;
void unbind() noexcept;

renderer::ChunkRenderer::ChunkRenderer()
{
  spdlog::info("ChunkRenderer initialized.");
  glGenVertexArrays(1, &_vertex_array_id);
}

renderer::ChunkRenderer::~ChunkRenderer()
{
  spdlog::info("ChunkRenderer destroyed.");

  for (const auto &buffer_id : _built_meshes_ids) {
    glDeleteBuffers(1, &buffer_id.second);
  }

  glDeleteVertexArrays(1, &_vertex_array_id);
}

void renderer::ChunkRenderer::update_mesh(unsigned int chunk_id, const renderer::Mesh &mesh) noexcept
{
  const auto &built_mesh_buffer{ _built_meshes_ids.find(chunk_id) };
  if (built_mesh_buffer != _built_meshes_ids.end()) {
    const auto buffer_id = built_mesh_buffer->second;
    glDeleteBuffers(1, &buffer_id);
  }

  GLuint buffer_id{};
  bind(_vertex_array_id);
  glGenBuffers(1, &buffer_id);
  glBindBuffer(GL_ARRAY_BUFFER, buffer_id);

  const auto &data = mesh.get_vertex_data();
  glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(renderer::Vertex), data.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  unbind();
  _built_meshes_ids.insert_or_assign(chunk_id, buffer_id);
  _meshes.insert_or_assign(chunk_id, mesh);
}

void renderer::ChunkRenderer::delete_mesh(unsigned int chunk_id) noexcept
{
  const auto &built_mesh_buffer{ _built_meshes_ids.find(chunk_id) };
  if (built_mesh_buffer != _built_meshes_ids.end()) {
    const auto buffer_id = built_mesh_buffer->second;
    glDeleteBuffers(1, &buffer_id);

    _meshes.erase(chunk_id);
    _built_meshes_ids.erase(chunk_id);
  }
}

void renderer::ChunkRenderer::render() const noexcept
{
  bind(_vertex_array_id);
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  for (const auto &mesh : _meshes) {
    const auto &buffer_id = _built_meshes_ids.find(mesh.first);
    if (buffer_id == _built_meshes_ids.end()) {
      spdlog::warn("No buffer id found for chunk [{}].", mesh.first);
      continue;
    }

    glBindBuffer(GL_ARRAY_BUFFER, buffer_id->second);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    spdlog::debug("Rendering chunk {} with {} vertices.", mesh.first, mesh.second.get_vertex_data_size());

    glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(mesh.second.get_vertex_data_size()));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  unbind();
}

inline void bind(GLuint vertex_array_id) noexcept
{
  glBindVertexArray(vertex_array_id);
}

inline void unbind() noexcept
{
  glBindVertexArray(0);
}
