#include "Generator.h"

using namespace world;

#include <Utils/PerlinNoise.h>

double noise_at(float x, float z) noexcept
{
  static siv::PerlinNoise   perlin{ 42 };
  static const std::int32_t octave = 16;
  static const double       fx{ chunks::CHUNK_SIZE_Y * 4 };
  static const double       fz{ chunks::CHUNK_SIZE_Y * 4 };

  return perlin.accumulatedOctaveNoise3D_0_1(static_cast<double>(x) / fx, 0, static_cast<double>(z) / fz, octave);
}


chunks::Chunk chunks::generate_chunk(int id, const glm::vec2 &position)
{
  chunks::Chunk chunk{ id, position };

  std::size_t chunk_size = CHUNK_SIZE_Y * CHUNK_SIZE_Y * CHUNK_SIZE_Y + CHUNK_SIZE_Z * CHUNK_SIZE_Z + CHUNK_SIZE_X;

  chunk.blocks.reserve(chunk_size);

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

            if (std::rand() % 1000 > 988) {
              block.hasThree = true;
            }

          } else {
            block.type = blocks::BlockType::DIRT;
          }
        }

        chunk.blocks.push_back(block);
      }
    }
  }

  /// TODO TREE
  // for (const auto &block : chunk.blocks) {
  //   if (block.hasThree) {
  //     const auto height = std::clamp(std::rand() % 5, 3, 5);

  //     auto leafSize = 2;

  //     // FIXME
  //     for (int x = -leafSize; x <= leafSize; ++x) {
  //       for (int z = -leafSize; z <= leafSize; ++z) {
  //         const auto newPosition = block.position + glm::vec3(x, height, z);
  //         update_block_type_at(chunk, newPosition, blocks::BlockType::LEAF);
  //       }
  //     }

  //     leafSize = 1;
  //     for (int x = -leafSize; x <= leafSize; ++x) {
  //       for (int z = -leafSize; z <= leafSize; ++z) {
  //         const auto newPosition = block.position + glm::vec3(x, height + 1, z);
  //         update_block_type_at(chunk, newPosition, blocks::BlockType::LEAF);
  //       }
  //     }

  //     const auto newPosition = block.position + glm::vec3(0, height + 2, 0);
  //     update_block_type_at(chunk, newPosition, blocks::BlockType::LEAF);

  //     for (auto i = 1; i <= height; i++) {
  //       update_block_type_at(chunk, block.position + glm::vec3(0, i, 0), blocks::BlockType::WOOD);
  //     }
  //   }
  // }

  return chunk;
}
