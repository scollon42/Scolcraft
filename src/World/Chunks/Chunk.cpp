#include "Chunk.h"

#include <spdlog/spdlog.h>
#include <glm/glm.hpp>

using namespace world;

chunks::Chunk::Chunk(int id, const glm::vec2 &position)
  : id(id), position(position)
{
}

std::vector<chunks::ChunkBlock> chunks::get_neighbours_blocks(const chunks::Chunk &chunk, const glm::vec3 &position) noexcept
{
  return {
    get_relative_block_at(chunk, position + glm::vec3{ -1, 0, 0 }),
    get_relative_block_at(chunk, position + glm::vec3{ 1, 0, 0 }),
    get_relative_block_at(chunk, position + glm::vec3{ 0, -1, 0 }),
    get_relative_block_at(chunk, position + glm::vec3{ 0, 1, 0 }),
    get_relative_block_at(chunk, position + glm::vec3{ 0, 0, -1 }),
    get_relative_block_at(chunk, position + glm::vec3{ 0, 0, 1 })
  };
}
[[maybe_unused]] const chunks::ChunkBlock &chunks::get_absolute_block_at(const chunks::Chunk &chunk, const glm::vec3 &position)
{
  return get_relative_block_at(chunk, to_relative_position(chunk, position));
}
const chunks::ChunkBlock &chunks::get_relative_block_at(const chunks::Chunk &chunk, const glm::vec3 &position)
{
  const auto index{ get_index_at(position) };

  if (index < 0 || static_cast<std::size_t>(index) >= chunk.blocks.size()) {
    spdlog::debug("Chunk [{}] : Block don't exists at relative position [{}, {}, {}] with index={}", chunk.id, position.x, position.y, position.z, index);
    return chunks::DEFAULT_BLOCK;
  }

  return chunk.blocks.at(static_cast<std::size_t>(index));
}

void chunks::update_block_type_at(chunks::Chunk &chunk, const glm::vec3 &position, blocks::BlockType blockType) noexcept
{
  const auto index{ get_index_at(position) };

  if (index < 0 || static_cast<std::size_t>(index) >= chunk.blocks.size()) {
    spdlog::debug("Chunk [{}] : Block don't exists at relative position [{}, {}, {}] with index={}", chunk.id, position.x, position.y, position.z, index);
  }

  try {
    auto &block    = chunk.blocks.at(static_cast<std::size_t>(index));
    block.type     = blockType;
    block.position = position;
  } catch (const std::out_of_range &oor) {
    spdlog::error("Out of range with chunk [{}] for position [{}, {}, {}] => [index={}].", chunk.id, position.x, position.y, position.z, index);
  }
}
