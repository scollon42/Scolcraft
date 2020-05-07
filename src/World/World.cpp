#include "World.h"

#include <spdlog/spdlog.h>

world::World world::World::generate_world(unsigned int iteration_number) noexcept
{
  world::World world{};

  for (unsigned int z = 0; z < iteration_number; z++) {
    for (unsigned int x = 0; x < iteration_number; x++) {
      world::chunk_id id{ z * iteration_number + x };
      auto chunk = world::Chunk::build_chunk(id, glm::vec3{ x, 0, z });

      world.add_chunk(chunk);
    }
  }

  spdlog::info("World has {} chunks.", world.get_chunk_count());

  return world;
}

const std::vector<world::Chunk> &world::World::get_chunks() const noexcept
{
  return this->_chunks;
}

std::size_t world::World::get_chunk_count() const noexcept
{
  return this->_chunks.size();
}

void world::World::add_chunk(const world::Chunk &chunk) noexcept
{
  this->_chunks.emplace_back(chunk);
}
