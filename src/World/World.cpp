#include "World.h"

#include <spdlog/spdlog.h>
#include <future>

std::mutex g_Mutex{};

static void gen_and_emplace_chunk(std::unordered_map<int, world::Chunk> *chunks, int x, int y) noexcept
{
  const auto index = world::get_index_at(glm::vec2{ x, y });

  world::Chunk chunk{ world::generate_chunk(index, glm::vec2{ x, y }) };

  {
    std::lock_guard<std::mutex> lock{ g_Mutex };
    chunks->emplace(std::make_pair(chunk.id, chunk));
  }
}

void world::World::build() noexcept
{
  constexpr int                  half_world = world::WORLD_SIZE / 2;
  std::vector<std::future<void>> futures{};

  for (int x = -half_world; x < half_world; x++) {
    for (int y = -half_world; y < half_world; y++) {
      futures.push_back(std::async(std::launch::async, gen_and_emplace_chunk, &_chunks, x, y));
    }
  }

  spdlog::info("World has {} chunks.", _chunks.size());
}


const world::Chunk &world::World::get_chunk_at_player(const glm::vec3 &position) const noexcept
{
  return _chunks.at(world::get_index_at_player(position));
}

std::vector<world::Chunk> world::World::get_chunks_around(const glm::vec3 &position, [[maybe_unused]] unsigned int radius = 1) const noexcept
{
  std::vector<world::Chunk> chunks{};

  const auto current_chunk{ get_chunk_at_player(position) };
  chunks.push_back(current_chunk);
  const auto current_chunk_position = current_chunk.position;
  const int  rad                    = static_cast<int>(radius);

  for (int x = -rad; x < rad; x++) {
    for (int y = -rad; y < rad; y++) {
      const auto pos = current_chunk_position + glm::vec2(x, y);

      chunks.push_back(_chunks.at(get_index_at(pos)));
    }
  }
  return chunks;
}
