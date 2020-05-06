#pragma once

#include <vector>
#include "Chunk.h"

namespace world {
class World
{
public:
  [[nodiscard]] static World generate_world(unsigned int iteration_number) noexcept;
  [[nodiscard]] std::size_t get_chunk_count() const noexcept;
  [[nodiscard]] const std::vector<Chunk> &get_chunks() const noexcept;
  void add_chunk(const Chunk &chunk) noexcept;

private:
  std::vector<Chunk> _chunks;
};
}// namespace world
