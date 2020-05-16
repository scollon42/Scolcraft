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

        const auto absolute_position = absolute_block_position(chunk, block);

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

glm::vec3 world::absolute_block_position(const world::Chunk &chunk, const world::Block &block) noexcept
{
  return glm::vec3{
    chunk.position.x * CHUNK_SIZE_X + block.position.x,
    0 * CHUNK_SIZE_Y + block.position.y,
    chunk.position.y * CHUNK_SIZE_Z + block.position.z
  };
}

bool world::is_next_to_air_block([[maybe_unused]] const world::Chunk &chunk, [[maybe_unused]] const world::Block &block) noexcept
{
  spdlog::error("world::is_next_to_air_block : Not implemented.");
  return false;
}

const world::Block &world::absolute_block_at(const world::Chunk &chunk, const glm::vec3 &position)
{
  const glm::vec3 relative_position{
    position.x - chunk.position.x - CHUNK_SIZE_X,
    position.y - 0 - CHUNK_SIZE_Y,
    position.z - chunk.position.y - CHUNK_SIZE_Z
  };

  const auto index = static_cast<std::size_t>(relative_position.z * CHUNK_SIZE_Z + relative_position.y * CHUNK_SIZE_Y + relative_position.y);

  if (index > chunk.blocks.size()) {
    throw std::runtime_error(fmt::format("Chunk [{}] : Block don't exists at absolute position [{}, {}, {}]", chunk.id, position.x, position.y, position.z));
  }

  return chunk.blocks.at(index);
}