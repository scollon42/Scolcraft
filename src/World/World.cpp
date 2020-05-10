#include "World.h"

#include <spdlog/spdlog.h>

constexpr int WORLD_SIZE = 5;

void world::World::build() noexcept
{
  for (int z = -WORLD_SIZE; z < WORLD_SIZE; z++) {
    for (int x = -WORLD_SIZE; x < WORLD_SIZE; x++) {
      const auto index = x + (z * WORLD_SIZE);
      world::Chunk chunk{ world::generate_chunk(index, glm::vec3{ x, 0, z }) };
      spdlog::info("Chunk [{}] has position [{}, {}, {}].", chunk.id, chunk.position.x, chunk.position.y, chunk.position.z);

      _chunks.emplace(index, chunk);
    }
  }

  spdlog::info("World has {} chunks.", _chunks.size());
}

const world::Chunk &world::World::get_chunk_at(const glm::vec3 &position) const noexcept
{
  const auto index = static_cast<int>(floorf(position.x / world::CHUNK_SIZE_Z) + floorf((position.z / world::CHUNK_SIZE_Z)) * WORLD_SIZE);
  return _chunks.at(index);
}

std::vector<world::Chunk> world::World::get_chunks_around(const glm::vec3 &position, [[maybe_unused]] unsigned int radius) const noexcept
{
  std::vector<world::Chunk> chunks{};
  chunks.emplace_back(get_chunk_at(position));
  //  chunks.emplace_back(get_chunk_at(position + glm::vec3{ world::CHUNK_SIZE_X, 0, 0 }));
  //  chunks.emplace_back(get_chunk_at(position + glm::vec3{ 0, 0, world::CHUNK_SIZE_Z }));
  //  chunks.emplace_back(get_chunk_at(position + glm::vec3{ world::CHUNK_SIZE_X, 0, world::CHUNK_SIZE_Z }));
  //  chunks.emplace_back(get_chunk_at(position + glm::vec3{ world::CHUNK_SIZE_X, 0, -world::CHUNK_SIZE_Z }));
  //  chunks.emplace_back(get_chunk_at(position + glm::vec3{ -world::CHUNK_SIZE_X, 0, world::CHUNK_SIZE_Z }));
  //  chunks.emplace_back(get_chunk_at(position + glm::vec3{ -world::CHUNK_SIZE_X, 0, 0 }));
  //  chunks.emplace_back(get_chunk_at(position + glm::vec3{ 0, 0, -world::CHUNK_SIZE_Z }));
  //  chunks.emplace_back(get_chunk_at(position + glm::vec3{ -world::CHUNK_SIZE_X, 0, -world::CHUNK_SIZE_Z }));


  return chunks;
}
