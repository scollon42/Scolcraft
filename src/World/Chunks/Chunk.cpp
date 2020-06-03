#include "Chunk.h"

#include <spdlog/spdlog.h>
#include <glm/glm.hpp>

#include <Utils/PerlinNoise.h>

using namespace world;

double noise_at(float x, float z) noexcept
{
  static siv::PerlinNoise   perlin{ 42 };
  static const std::int32_t octave = 16;
  static const double       fx{ chunks::CHUNK_SIZE_Y * 4 };
  static const double       fz{ chunks::CHUNK_SIZE_Y * 4 };

  return perlin.accumulatedOctaveNoise3D_0_1(static_cast<double>(x) / fx, 0, static_cast<double>(z) / fz, octave);
}

chunks::Chunk::Chunk(int id, const glm::vec2 &position)
  : id(id), position(position)
{
}

chunks::Chunk chunks::generate_chunk(int id, const glm::vec2 &position)
{
  chunks::Chunk chunk{ id, position };

  for (int z = 0; z < CHUNK_SIZE_Z; z++) {
    for (int y = 0; y < CHUNK_SIZE_Y; y++) {
      for (int x = 0; x < CHUNK_SIZE_X; x++) {

        ChunkBlock block{
          blocks::BlockType::AIR,
          glm::vec3(x, y, z)
        };

        const auto absolute_position = to_absolute_position(chunk, block.position);

        const auto height = noise_at(absolute_position.x, absolute_position.z) * CHUNK_SIZE_Y;

        if (y < height) {
          if (y + 1 >= height) {
            block.type = blocks::BlockType::GRASS;
          } else {
            block.type = blocks::BlockType::DIRT;
          }
        }

        chunk.blocks.push_back(block);
      }
    }
  }

  return chunk;
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
    spdlog::debug("Chunk [{}] : Block don't exists at absolute position [{}, {}, {}]", chunk.id, position.x, position.y, position.z);
    return chunks::DEFAULT_BLOCK;
  }

  return chunk.blocks.at(static_cast<std::size_t>(index));
}