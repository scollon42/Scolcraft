#include "Chunk.h"

#include <spdlog/spdlog.h>
#include <glm/glm.hpp>

#include "Utils/PerlinNoise.h"
#include "Renderer/Mesh.h"

double noise_at(float x, float z) noexcept
{
  static siv::PerlinNoise perlin{ 42 };
  static const std::int32_t octave = 16;
  static const double fx{ 1024 };
  static const double fz{ 1024 };

  return perlin.accumulatedOctaveNoise3D_0_1(static_cast<double>(x) / fx, 0, static_cast<double>(z) / fz, octave);
}

world::Chunk world::generate_chunk(unsigned int id, const glm::vec3 &position)
{
  world::Chunk chunk{};

  chunk.id = id;
  chunk.position = position;

  constexpr auto BLOCK_CHUNK_SIZE = CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z;

  std::vector<vertex_data> chunk_vertices{};

  chunk_vertices.reserve(BLOCK_CHUNK_SIZE);

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
    chunk.position.y * CHUNK_SIZE_Y + block.position.y,
    chunk.position.z * CHUNK_SIZE_Z + block.position.z
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
    position.y - chunk.position.y - CHUNK_SIZE_Y,
    position.z - chunk.position.z - CHUNK_SIZE_Z
  };

  const auto index = static_cast<std::size_t>(relative_position.z * CHUNK_SIZE_Z + relative_position.y * CHUNK_SIZE_Y + relative_position.y);

  if (index > chunk.blocks.size()) {
    throw std::runtime_error(fmt::format("Chunk [{}] : Block don't exists at absolute position [{}, {}, {}]", chunk.id, position.x, position.y, position.z));
  }

  return chunk.blocks.at(index);
}

renderer::Mesh world::get_chunk_mesh(const world::Chunk &chunk) noexcept
{
  //  std::vector<vertex_data> data;
  renderer::Mesh mesh{};
  mesh.init();
  //FIXME this is ugly as fuck
  //  data.reserve(world::CHUNK_SIZE_X * world::CHUNK_SIZE_Y * world::CHUNK_SIZE_Z * world::BLOCK_SIZE * world::BLOCK_SIZE);
  for (const auto &block : chunk.blocks) {
    if (!world::is_visible_block_type(block.type)) {
      //      spdlog::info("Chunk [{}] : Block at [{}, {}, {}] is not visible.", chunk.id, block.position.x, block.position.y, block.position.z);
      continue;
    }

    if (!(block.position.x - 1 < 0
          || block.position.y - 1 < 0
          || block.position.z - 1 < 0
          || block.position.x + 1 >= CHUNK_SIZE_X
          || block.position.y + 1 >= CHUNK_SIZE_Y
          || block.position.z + 1 >= CHUNK_SIZE_Z)) {

      const auto &neighbour1 = chunk.blocks.at(static_cast<std::size_t>((block.position.x - 1) + CHUNK_SIZE_X * (block.position.y + CHUNK_SIZE_Y * block.position.z)));
      const auto &neighbour2 = chunk.blocks.at(static_cast<std::size_t>((block.position.x + 1) + CHUNK_SIZE_X * (block.position.y + CHUNK_SIZE_Y * block.position.z)));
      const auto &neighbour3 = chunk.blocks.at(static_cast<std::size_t>(block.position.x + CHUNK_SIZE_X * (block.position.y - 1 + CHUNK_SIZE_Y * block.position.z)));
      const auto &neighbour4 = chunk.blocks.at(static_cast<std::size_t>(block.position.x + CHUNK_SIZE_X * (block.position.y + 1 + CHUNK_SIZE_Y * block.position.z)));
      const auto &neighbour5 = chunk.blocks.at(static_cast<std::size_t>(block.position.x + CHUNK_SIZE_X * (block.position.y + CHUNK_SIZE_Y * (block.position.z - 1))));
      const auto &neighbour6 = chunk.blocks.at(static_cast<std::size_t>(block.position.x + CHUNK_SIZE_X * (block.position.y + CHUNK_SIZE_Y * (block.position.z + 1))));

      if (neighbour1.type == BlockType::DIRT
          && neighbour2.type == BlockType::DIRT
          && neighbour3.type == BlockType::DIRT
          && neighbour4.type == BlockType::DIRT
          && neighbour5.type == BlockType::DIRT
          && neighbour6.type == BlockType::DIRT) {
        //        spdlog::info("Chunk [{}] : Block at [{}, {}, {}] should not be drawn", chunk.id, block.position.x, block.position.y, block.position.z);
        continue;
      }
    }

    const auto &block_data = world::get_block_vertex_data(chunk, block);

    mesh.insert_data(block_data);
  }

  return mesh;
}

std::vector<renderer::Vertex> world::get_block_vertex_data(const world::Chunk &chunk, const world::Block &block)
{
  std::vector<renderer::Vertex> block_vertex_data{};
  //negative y
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk.position.x * CHUNK_SIZE_X + block.position.x, chunk.position.y * CHUNK_SIZE_Y + block.position.y, chunk.position.z * CHUNK_SIZE_Z + block.position.z }, glm::vec3{ -1, 0, 0 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk.position.x * CHUNK_SIZE_X + block.position.x, chunk.position.y * CHUNK_SIZE_Y + block.position.y, chunk.position.z * CHUNK_SIZE_Z + block.position.z + 1 }, glm::vec3{ -1, 0, 0 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk.position.x * CHUNK_SIZE_X + block.position.x, chunk.position.y * CHUNK_SIZE_Y + block.position.y + 1, chunk.position.z * CHUNK_SIZE_Z + block.position.z }, glm::vec3{ -1, 0, 0 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk.position.x * CHUNK_SIZE_X + block.position.x, chunk.position.y * CHUNK_SIZE_Y + block.position.y + 1, chunk.position.z * CHUNK_SIZE_Z + block.position.z }, glm::vec3{ -1, 0, 0 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk.position.x * CHUNK_SIZE_X + block.position.x, chunk.position.y * CHUNK_SIZE_Y + block.position.y, chunk.position.z * CHUNK_SIZE_Z + block.position.z + 1 }, glm::vec3{ -1, 0, 0 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk.position.x * CHUNK_SIZE_X + block.position.x, chunk.position.y * CHUNK_SIZE_Y + block.position.y + 1, chunk.position.z * CHUNK_SIZE_Z + block.position.z + 1 }, glm::vec3{ -1, 0, 0 } });
  // positive x
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk.position.x * CHUNK_SIZE_X + block.position.x + 1, chunk.position.y * CHUNK_SIZE_Y + block.position.y, chunk.position.z * CHUNK_SIZE_Z + block.position.z }, glm::vec3{ 1, 0, 0 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk.position.x * CHUNK_SIZE_X + block.position.x + 1, chunk.position.y * CHUNK_SIZE_Y + block.position.y + 1, chunk.position.z * CHUNK_SIZE_Z + block.position.z }, glm::vec3{ 1, 0, 0 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk.position.x * CHUNK_SIZE_X + block.position.x + 1, chunk.position.y * CHUNK_SIZE_Y + block.position.y, chunk.position.z * CHUNK_SIZE_Z + block.position.z + 1 }, glm::vec3{ 1, 0, 0 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk.position.x * CHUNK_SIZE_X + block.position.x + 1, chunk.position.y * CHUNK_SIZE_Y + block.position.y + 1, chunk.position.z * CHUNK_SIZE_Z + block.position.z }, glm::vec3{ 1, 0, 0 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk.position.x * CHUNK_SIZE_X + block.position.x + 1, chunk.position.y * CHUNK_SIZE_Y + block.position.y + 1, chunk.position.z * CHUNK_SIZE_Z + block.position.z + 1 }, glm::vec3{ 1, 0, 0 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk.position.x * CHUNK_SIZE_X + block.position.x + 1, chunk.position.y * CHUNK_SIZE_Y + block.position.y, chunk.position.z * CHUNK_SIZE_Z + block.position.z + 1 }, glm::vec3{ 1, 0, 0 } });
  //negative y
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk.position.x * CHUNK_SIZE_X + block.position.x, chunk.position.y * CHUNK_SIZE_Y + block.position.y, chunk.position.z * CHUNK_SIZE_Z + block.position.z }, glm::vec3{ 0, -1, 0 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk.position.x * CHUNK_SIZE_X + block.position.x + 1, chunk.position.y * CHUNK_SIZE_Y + block.position.y, chunk.position.z * CHUNK_SIZE_Z + block.position.z }, glm::vec3{ 0, -1, 0 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk.position.x * CHUNK_SIZE_X + block.position.x, chunk.position.y * CHUNK_SIZE_Y + block.position.y, chunk.position.z * CHUNK_SIZE_Z + block.position.z + 1 }, glm::vec3{ 0, -1, 0 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk.position.x * CHUNK_SIZE_X + block.position.x + 1, chunk.position.y * CHUNK_SIZE_Y + block.position.y, chunk.position.z * CHUNK_SIZE_Z + block.position.z }, glm::vec3{ 0, -1, 0 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk.position.x * CHUNK_SIZE_X + block.position.x + 1, chunk.position.y * CHUNK_SIZE_Y + block.position.y, chunk.position.z * CHUNK_SIZE_Z + block.position.z + 1 }, glm::vec3{ 0, -1, 0 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk.position.x * CHUNK_SIZE_X + block.position.x, chunk.position.y * CHUNK_SIZE_Y + block.position.y, chunk.position.z * CHUNK_SIZE_Z + block.position.z + 1 }, glm::vec3{ 0, -1, 0 } });
  //positive y
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk.position.x * CHUNK_SIZE_X + block.position.x, chunk.position.y * CHUNK_SIZE_Y + block.position.y + 1, chunk.position.z * CHUNK_SIZE_Z + block.position.z }, glm::vec3{ 0, 1, 0 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk.position.x * CHUNK_SIZE_X + block.position.x, chunk.position.y * CHUNK_SIZE_Y + block.position.y + 1, chunk.position.z * CHUNK_SIZE_Z + block.position.z + 1 }, glm::vec3{ 0, 1, 0 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk.position.x * CHUNK_SIZE_X + block.position.x + 1, chunk.position.y * CHUNK_SIZE_Y + block.position.y + 1, chunk.position.z * CHUNK_SIZE_Z + block.position.z }, glm::vec3{ 0, 1, 0 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk.position.x * CHUNK_SIZE_X + block.position.x + 1, chunk.position.y * CHUNK_SIZE_Y + block.position.y + 1, chunk.position.z * CHUNK_SIZE_Z + block.position.z }, glm::vec3{ 0, 1, 0 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk.position.x * CHUNK_SIZE_X + block.position.x, chunk.position.y * CHUNK_SIZE_Y + block.position.y + 1, chunk.position.z * CHUNK_SIZE_Z + block.position.z + 1 }, glm::vec3{ 0, 1, 0 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk.position.x * CHUNK_SIZE_X + block.position.x + 1, chunk.position.y * CHUNK_SIZE_Y + block.position.y + 1, chunk.position.z * CHUNK_SIZE_Z + block.position.z + 1 }, glm::vec3{ 0, 1, 0 } });
  //negative z
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk.position.x * CHUNK_SIZE_X + block.position.x, chunk.position.y * CHUNK_SIZE_Y + block.position.y, chunk.position.z * CHUNK_SIZE_Z + block.position.z }, glm::vec3{ 0, 0, -1 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk.position.x * CHUNK_SIZE_X + block.position.x, chunk.position.y * CHUNK_SIZE_Y + block.position.y + 1, chunk.position.z * CHUNK_SIZE_Z + block.position.z }, glm::vec3{ 0, 0, -1 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk.position.x * CHUNK_SIZE_X + block.position.x + 1, chunk.position.y * CHUNK_SIZE_Y + block.position.y, chunk.position.z * CHUNK_SIZE_Z + block.position.z }, glm::vec3{ 0, 0, -1 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk.position.x * CHUNK_SIZE_X + block.position.x, chunk.position.y * CHUNK_SIZE_Y + block.position.y + 1, chunk.position.z * CHUNK_SIZE_Z + block.position.z }, glm::vec3{ 0, 0, -1 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk.position.x * CHUNK_SIZE_X + block.position.x + 1, chunk.position.y * CHUNK_SIZE_Y + block.position.y + 1, chunk.position.z * CHUNK_SIZE_Z + block.position.z }, glm::vec3{ 0, 0, -1 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk.position.x * CHUNK_SIZE_X + block.position.x + 1, chunk.position.y * CHUNK_SIZE_Y + block.position.y, chunk.position.z * CHUNK_SIZE_Z + block.position.z }, glm::vec3{ 0, 0, -1 } });
  //positive z
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk.position.x * CHUNK_SIZE_X + block.position.x, chunk.position.y * CHUNK_SIZE_Y + block.position.y, chunk.position.z * CHUNK_SIZE_Z + block.position.z + 1 }, glm::vec3{ 0, 0, 1 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk.position.x * CHUNK_SIZE_X + block.position.x + 1, chunk.position.y * CHUNK_SIZE_Y + block.position.y, chunk.position.z * CHUNK_SIZE_Z + block.position.z + 1 }, glm::vec3{ 0, 0, 1 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk.position.x * CHUNK_SIZE_X + block.position.x, chunk.position.y * CHUNK_SIZE_Y + block.position.y + 1, chunk.position.z * CHUNK_SIZE_Z + block.position.z + 1 }, glm::vec3{ 0, 0, 1 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk.position.x * CHUNK_SIZE_X + block.position.x, chunk.position.y * CHUNK_SIZE_Y + block.position.y + 1, chunk.position.z * CHUNK_SIZE_Z + block.position.z + 1 }, glm::vec3{ 0, 0, 1 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk.position.x * CHUNK_SIZE_X + block.position.x + 1, chunk.position.y * CHUNK_SIZE_Y + block.position.y, chunk.position.z * CHUNK_SIZE_Z + block.position.z + 1 }, glm::vec3{ 0, 0, 1 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk.position.x * CHUNK_SIZE_X + block.position.x + 1, chunk.position.y * CHUNK_SIZE_Y + block.position.y + 1, chunk.position.z * CHUNK_SIZE_Z + block.position.z + 1 }, glm::vec3{ 0, 0, 1 } });

  return block_vertex_data;
}

void world::build_mesh(world::Chunk &chunk)
{
  chunk.mesh = world::get_chunk_mesh(chunk);
  chunk.mesh.build();
}

void world::draw(const world::Chunk &chunk)
{
  chunk.mesh.render();
}
