#pragma once

#include <vector>
#include <unordered_map>
#include <glm/vec3.hpp>
#include "Chunk.h"

namespace world {

constexpr int WORLD_SIZE = 20;

class World final
{
public:
  void build() noexcept;

  [[nodiscard]] const Chunk &get_chunk_at_player(const glm::vec3 &position) const noexcept;
  [[nodiscard]] std::vector<Chunk> get_chunks_around(const glm::vec3 &position, unsigned int radius) const noexcept;

private:
  std::unordered_map<int, Chunk> _chunks;
};

inline int get_index_at_player(const glm::vec3 &position) noexcept
{
  return static_cast<int>(floorf(position.x / CHUNK_SIZE_X) * WORLD_SIZE + floorf(position.z / CHUNK_SIZE_Z));
}

inline int get_index_at(const glm::vec2 &position) noexcept
{
  return static_cast<int>(position.x * WORLD_SIZE + position.y);
}

}// namespace world
