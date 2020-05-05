#include "World.h"

#include <spdlog/spdlog.h>

world::World world::generate_world() noexcept
{
  world::World world{};

  constexpr auto WORLD_SIZE = 5;

  for (unsigned int z = 0; z < WORLD_SIZE; z++) {
    for (unsigned int x = 0; x < WORLD_SIZE; x++) {
      world::Chunk chunk{ world::generate_chunk(z * WORLD_SIZE + x, glm::vec3{ x, 0, z }) };
      spdlog::info("Chunk [{}] has position [{}, {}, {}].", chunk.id, chunk.position.x, chunk.position.y, chunk.position.z);

      world::build_mesh(chunk);
      world.chunks.emplace_back(chunk);
    }
  }

  spdlog::info("World has {} chunks.", world.chunks.size());

  return world;
}
