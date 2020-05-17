#include "Chunk.h"

#include <spdlog/spdlog.h>
#include <glm/glm.hpp>

#include <Utils/PerlinNoise.h>
#include <Renderer/Mesh.h>
#include <Renderer/BlockFace.h>

double noise_at(float x, float z) noexcept
{
  static siv::PerlinNoise perlin{ 42 };
  static const std::int32_t octave = 16;
  static const double fx{ world::CHUNK_SIZE_Y * 4 };
  static const double fz{ world::CHUNK_SIZE_Y * 4 };

  return perlin.accumulatedOctaveNoise3D_0_1(static_cast<double>(x) / fx, 0, static_cast<double>(z) / fz, octave);
}

world::Chunk::Chunk(int id, const glm::vec2 &position)
  : id(id), position(position)
{
}

world::Chunk world::generate_chunk(int id, const glm::vec2 &position)
{
  world::Chunk chunk{ id, position };

  //filling vertex buffer dude
  for (int z = 0; z < CHUNK_SIZE_Z; z++) {
    for (int y = 0; y < CHUNK_SIZE_Y; y++) {
      for (int x = 0; x < CHUNK_SIZE_X; x++) {

        Block block{
          BlockType::AIR,
          glm::vec3(x, y, z)
        };

        const auto absolute_position = to_absolute_position(chunk, block.position);

        const auto height = noise_at(absolute_position.x, absolute_position.z) * CHUNK_SIZE_Y;

        if (y < height) {
          block.type = BlockType::DIRT;
        }

        chunk.blocks.emplace_back(block);
      }
    }
  }

  return chunk;
}

std::vector<world::Block> world::get_neighbours_blocks(const world::Chunk &chunk, const glm::vec3 &position) noexcept
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

const world::Block &world::get_absolute_block_at(const world::Chunk &chunk, const glm::vec3 &position)
{
  return get_relative_block_at(chunk, to_relative_position(chunk, position));
}

const world::Block &world::get_relative_block_at(const world::Chunk &chunk, const glm::vec3 &position)
{
  const auto index{ get_index_at(position) };

  if (index >= chunk.blocks.size()) {
    spdlog::warn("Chunk [{}] : Block don't exists at absolute position [{}, {}, {}]", chunk.id, position.x, position.y, position.z);
    return world::DEFAULT_BLOCK;
  }

  return chunk.blocks.at(index);
}
