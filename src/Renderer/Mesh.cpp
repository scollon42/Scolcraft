#include "Mesh.h"

const std::vector<renderer::Vertex> &renderer::Mesh::get_vertex_data() const noexcept
{
  return _vertex_data;
}

int renderer::Mesh::get_vertex_data_size() const noexcept
{
  return static_cast<int>(_vertex_data.size());
}

void renderer::Mesh::add_vertex(const renderer::Vertex &vertex) noexcept
{
  _vertex_data.push_back(vertex);
}

void renderer::Mesh::update_vertex_data(std::vector<Vertex> vertex_data) noexcept
{
  _vertex_data.clear();
  _vertex_data = std::move(vertex_data);
}

void renderer::Mesh::insert_vertex_data(const std::vector<Vertex> &vertex_data) noexcept
{
  _vertex_data.insert(_vertex_data.end(), vertex_data.begin(), vertex_data.end());
}
