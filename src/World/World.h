#pragma once

#include <vector>
#include <unordered_map>
#include <glm/vec3.hpp>
#include "Chunk.h"

namespace world {
class World final
{
public:
  void build() noexcept;

  const Chunk &get_chunk_at(const glm::vec3 &position) const noexcept;
  std::vector<Chunk> get_chunks_around(const glm::vec3 &position, unsigned int radius) const noexcept;

private:
  std::unordered_map<int, Chunk> _chunks;
};

}// namespace world
