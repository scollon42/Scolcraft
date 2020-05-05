#include "Mesh.h"
#include <glad/glad.h>

#include <spdlog/spdlog.h>

void renderer::Mesh::init() noexcept
{
  glGenVertexArrays(1, &this->_vertex_array);
  glGenBuffers(1, &this->_vertex_buffer);
}

void renderer::Mesh::destroy() noexcept
{
  glDeleteVertexArrays(1, &this->_vertex_array);
  glDeleteBuffers(1, &this->_vertex_buffer);
}

void renderer::Mesh::update(const std::vector<renderer::Vertex> &vertices) noexcept
{
  this->_vertices = vertices;
}

void renderer::Mesh::insert_data(const std::vector<renderer::Vertex> &vertices) noexcept
{
  this->_vertices.insert(this->_vertices.end(), vertices.begin(), vertices.end());
}

void renderer::Mesh::build() const noexcept
{
  bind();
  glBufferData(GL_ARRAY_BUFFER, this->_vertices.size() * sizeof(renderer::Vertex), this->_vertices.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  unbind();
}

void renderer::Mesh::bind() const noexcept
{
  glBindVertexArray(this->_vertex_array);
  glBindBuffer(GL_ARRAY_BUFFER, this->_vertex_buffer);
}

void renderer::Mesh::unbind() const noexcept
{
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void renderer::Mesh::render() const noexcept
{
  spdlog::info("Mesh {} rendering {} vertices", this->_vertex_array, this->_vertices.size());
  bind();
  glDrawArrays(GL_TRIANGLES, 0, this->_vertices.size());
  unbind();
}
