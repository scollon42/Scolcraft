#pragma once

#include <vector>
#include "Vertex.h"
#include <memory>

namespace renderer {
class Mesh final
{
public:
  [[nodiscard]] const std::vector<Vertex> &get_vertex_data() const noexcept;
  [[nodiscard]] int                        get_vertex_data_size() const noexcept;


  [[maybe_unused]] void add_vertex(const Vertex &vertex) noexcept;
  [[maybe_unused]] void update_vertex_data(std::vector<Vertex> vertex_data) noexcept;
  void                  insert_vertex_data(const std::vector<Vertex> &vertex_data) noexcept;

private:
  std::vector<Vertex> _vertex_data;
};

}// namespace renderer
