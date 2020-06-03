#pragma once

#include <vector>
#include <unordered_map>
#include <glm/vec3.hpp>
#include <World/Chunks/Chunk.h>
#include <World/Blocks/BlockType.h>
#include <World/Blocks/Block.h>

namespace world {
constexpr int WORLD_SIZE = 10;

class World final
{
public:
  void build() noexcept;

  [[nodiscard]] const chunks::Chunk &                                       get_chunk_at_player(const glm::vec3 &position) const noexcept;
  [[nodiscard]] std::vector<chunks::Chunk>                                  get_chunks_around(const glm::vec3 &position, unsigned int radius) const noexcept;
  [[nodiscard]] const std::unordered_map<blocks::BlockType, blocks::Block> &get_block_data() const noexcept;

private:
  std::unordered_map<int, chunks::Chunk>               _chunks;
  std::unordered_map<blocks::BlockType, blocks::Block> _block_data;
};

inline int get_index_at_player(const glm::vec3 &position) noexcept
{
  return static_cast<int>(floorf(position.x / chunks::CHUNK_SIZE_X) * WORLD_SIZE + floorf(position.z / chunks::CHUNK_SIZE_Z));
}

inline int get_index_at(const glm::vec2 &position) noexcept
{
  return static_cast<int>(position.x * WORLD_SIZE + position.y);
}

}// namespace world
