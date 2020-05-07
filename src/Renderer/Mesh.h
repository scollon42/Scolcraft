#pragma once

#include <vector>
#include "Vertex.h"

namespace renderer {
//struct Vertex
//{
//  float vertices[3];
//  float normals[3];
//};

class Mesh
{
public:
  void init() noexcept;
  void destroy() noexcept;
  void update(const std::vector<Vertex> &vertices) noexcept;
  void insert_data(const std::vector<Vertex> &vertices) noexcept;
  void build() const noexcept;
  void bind() const noexcept;
  void unbind() const noexcept;
  void render() const noexcept;

private:
  unsigned int _vertex_array;
  unsigned int _vertex_buffer;
  std::vector<Vertex> _vertices;
};

}// namespace renderer
