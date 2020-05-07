#include "Chunk.h"

#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <utility>

#include "Utils/PerlinNoise.h"
#include "Renderer/Mesh.h"

// FIXME refactor that
double noise_at(float x, float z) noexcept
{
  static siv::PerlinNoise perlin{ 42 };
  static const std::int32_t octave = 16;
  static const double fx{ 1024 };
  static const double fz{ 1024 };

  return perlin.accumulatedOctaveNoise3D_0_1(static_cast<double>(x) / fx, 0, static_cast<double>(z) / fz, octave);
}

[[nodiscard]] glm::vec3 absolute_block_position(const world::Chunk &chunk, const world::Block &block)
{
  const auto &chunk_position = chunk.get_position();

  return glm::vec3{
    chunk_position.x * world::CHUNK_SIZE_X + block.position.x,
    chunk_position.y * world::CHUNK_SIZE_Y + block.position.y,
    chunk_position.z * world::CHUNK_SIZE_Z + block.position.z
  };
}

world::Chunk::Chunk(world::chunk_id id, const glm::vec3 &position) : _id(id), _position(position)
{
}

world::Chunk::Chunk(world::chunk_id id, const glm::vec3 &position, std::vector<Block> blocks) : _id(id), _position(position), _blocks(std::move(blocks))
{
}

world::Chunk world::Chunk::build_chunk(world::chunk_id id, const glm::vec3 &position)
{
  world::Chunk chunk{ id, position };

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

        spdlog::info("Block of type {} is at {}, {}, {}", block.type == BlockType::AIR ? "AIR" : "DIRT", block.position.x, block.position.y, block.position.z);

        chunk.add_block(block);
      }
    }
  }

  return chunk;
}

world::chunk_id world::Chunk::get_chunk_id() const noexcept
{
  return this->_id;
}

const glm::vec3 &world::Chunk::get_position() const noexcept
{
  return this->_position;
}

const world::Block &world::Chunk::get_block_at_absolute([[maybe_unused]] const glm::vec3 &position) const
{
  const glm::vec3 relative_position{
    position.x - this->_position.x - CHUNK_SIZE_X,
    position.y - this->_position.y - CHUNK_SIZE_Y,
    position.z - this->_position.z - CHUNK_SIZE_Z
  };

  const auto index = static_cast<std::size_t>(relative_position.x + CHUNK_SIZE_X * (relative_position.y + CHUNK_SIZE_Y * relative_position.z));

  if (index > this->_blocks.size()) {
    throw std::runtime_error(fmt::format("Chunk [{}] : Block don't exists at absolute position [{}, {}, {}]", this->_id, position.x, position.y, position.z));
  }

  return this->_blocks.at(index);
}

const world::Block &world::Chunk::get_block_at_relative([[maybe_unused]] const glm::vec3 &position) const
{
  const auto index = static_cast<std::size_t>(position.x + CHUNK_SIZE_X * (position.y + CHUNK_SIZE_Y * position.z));

  if (index > this->_blocks.size()) {
    throw std::runtime_error(fmt::format("Chunk [{}] : Block don't exists at relative position [{}, {}, {}]", this->_id, position.x, position.y, position.z));
  }

  return this->_blocks.at(index);
}

const std::vector<world::Block> &world::Chunk::get_blocks() const noexcept
{
  return this->_blocks;
}

inline void world::Chunk::add_block(world::Block &&block) noexcept
{
  this->_blocks.emplace_back(block);
}

void world::Chunk::add_block(const world::Block &block) noexcept
{
  this->_blocks.emplace_back(block);
}
