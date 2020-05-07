#include "ChunkMeshBuilder.h"

#include <spdlog/spdlog.h>

[[nodiscard]] std::vector<renderer::Vertex> get_block_vertex_data(const world::Chunk &chunk, const world::Block &block) noexcept;

renderer::Mesh renderer::ChunkMeshBuilder::build(const world::Chunk &chunk) noexcept
{
  //  std::vector<vertex_data> data;
  renderer::Mesh mesh{};
  mesh.init();
  //FIXME this is ugly as fuck
  //  data.reserve(world::CHUNK_SIZE_X * world::CHUNK_SIZE_Y * world::CHUNK_SIZE_Z * world::BLOCK_SIZE * world::BLOCK_SIZE);
  for (const auto &block : chunk.get_blocks()) {
    if (!world::is_visible_block_type(block.type)) {
      //      spdlog::info("Chunk [{}] : Block at [{}, {}, {}] is not visible.", chunk.id, block.position.x, block.position.y, block.position.z);
      continue;
    }

    if (!(block.position.x - 1 < 0
          || block.position.y - 1 < 0
          || block.position.z - 1 < 0
          || block.position.x + 1 >= world::CHUNK_SIZE_X
          || block.position.y + 1 >= world::CHUNK_SIZE_Y
          || block.position.z + 1 >= world::CHUNK_SIZE_Z)) {

      const auto &neighbour1 = chunk.get_block_at_relative(glm::vec3(block.position.x - 1, block.position.y, block.position.z));
      const auto &neighbour2 = chunk.get_block_at_relative(glm::vec3(block.position.x + 1, block.position.y, block.position.z));
      const auto &neighbour3 = chunk.get_block_at_relative(glm::vec3(block.position.x, block.position.y - 1, block.position.z));
      const auto &neighbour4 = chunk.get_block_at_relative(glm::vec3(block.position.x, block.position.y + 1, block.position.z));
      const auto &neighbour5 = chunk.get_block_at_relative(glm::vec3(block.position.x, block.position.y, block.position.z - 1));
      const auto &neighbour6 = chunk.get_block_at_relative(glm::vec3(block.position.x, block.position.y, block.position.z + 1));

      if (neighbour1.type == world::BlockType::DIRT
          && neighbour2.type == world::BlockType::DIRT
          && neighbour3.type == world::BlockType::DIRT
          && neighbour4.type == world::BlockType::DIRT
          && neighbour5.type == world::BlockType::DIRT
          && neighbour6.type == world::BlockType::DIRT) {
        spdlog::info("Chunk [{}] : Block at [{}, {}, {}] should not be drawn", chunk.get_chunk_id(), block.position.x, block.position.y, block.position.z);
        continue;
      }
    }

    const auto &block_data = get_block_vertex_data(chunk, block);

    mesh.insert_data(block_data);
  }

  return mesh;
}

//FIXME fix that
std::vector<renderer::Vertex> get_block_vertex_data(const world::Chunk &chunk, const world::Block &block) noexcept
{
  std::vector<renderer::Vertex> block_vertex_data{};

  const auto &chunk_position = chunk.get_position();
  //negative y
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk_position.x * world::CHUNK_SIZE_X + block.position.x, chunk_position.y * world::CHUNK_SIZE_Y + block.position.y, chunk_position.z * world::CHUNK_SIZE_Z + block.position.z }, glm::vec3{ -1, 0, 0 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk_position.x * world::CHUNK_SIZE_X + block.position.x, chunk_position.y * world::CHUNK_SIZE_Y + block.position.y, chunk_position.z * world::CHUNK_SIZE_Z + block.position.z + 1 }, glm::vec3{ -1, 0, 0 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk_position.x * world::CHUNK_SIZE_X + block.position.x, chunk_position.y * world::CHUNK_SIZE_Y + block.position.y + 1, chunk_position.z * world::CHUNK_SIZE_Z + block.position.z }, glm::vec3{ -1, 0, 0 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk_position.x * world::CHUNK_SIZE_X + block.position.x, chunk_position.y * world::CHUNK_SIZE_Y + block.position.y + 1, chunk_position.z * world::CHUNK_SIZE_Z + block.position.z }, glm::vec3{ -1, 0, 0 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk_position.x * world::CHUNK_SIZE_X + block.position.x, chunk_position.y * world::CHUNK_SIZE_Y + block.position.y, chunk_position.z * world::CHUNK_SIZE_Z + block.position.z + 1 }, glm::vec3{ -1, 0, 0 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk_position.x * world::CHUNK_SIZE_X + block.position.x, chunk_position.y * world::CHUNK_SIZE_Y + block.position.y + 1, chunk_position.z * world::CHUNK_SIZE_Z + block.position.z + 1 }, glm::vec3{ -1, 0, 0 } });
  // positive x
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk_position.x * world::CHUNK_SIZE_X + block.position.x + 1, chunk_position.y * world::CHUNK_SIZE_Y + block.position.y, chunk_position.z * world::CHUNK_SIZE_Z + block.position.z }, glm::vec3{ 1, 0, 0 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk_position.x * world::CHUNK_SIZE_X + block.position.x + 1, chunk_position.y * world::CHUNK_SIZE_Y + block.position.y + 1, chunk_position.z * world::CHUNK_SIZE_Z + block.position.z }, glm::vec3{ 1, 0, 0 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk_position.x * world::CHUNK_SIZE_X + block.position.x + 1, chunk_position.y * world::CHUNK_SIZE_Y + block.position.y, chunk_position.z * world::CHUNK_SIZE_Z + block.position.z + 1 }, glm::vec3{ 1, 0, 0 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk_position.x * world::CHUNK_SIZE_X + block.position.x + 1, chunk_position.y * world::CHUNK_SIZE_Y + block.position.y + 1, chunk_position.z * world::CHUNK_SIZE_Z + block.position.z }, glm::vec3{ 1, 0, 0 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk_position.x * world::CHUNK_SIZE_X + block.position.x + 1, chunk_position.y * world::CHUNK_SIZE_Y + block.position.y + 1, chunk_position.z * world::CHUNK_SIZE_Z + block.position.z + 1 }, glm::vec3{ 1, 0, 0 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk_position.x * world::CHUNK_SIZE_X + block.position.x + 1, chunk_position.y * world::CHUNK_SIZE_Y + block.position.y, chunk_position.z * world::CHUNK_SIZE_Z + block.position.z + 1 }, glm::vec3{ 1, 0, 0 } });
  //negative y
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk_position.x * world::CHUNK_SIZE_X + block.position.x, chunk_position.y * world::CHUNK_SIZE_Y + block.position.y, chunk_position.z * world::CHUNK_SIZE_Z + block.position.z }, glm::vec3{ 0, -1, 0 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk_position.x * world::CHUNK_SIZE_X + block.position.x + 1, chunk_position.y * world::CHUNK_SIZE_Y + block.position.y, chunk_position.z * world::CHUNK_SIZE_Z + block.position.z }, glm::vec3{ 0, -1, 0 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk_position.x * world::CHUNK_SIZE_X + block.position.x, chunk_position.y * world::CHUNK_SIZE_Y + block.position.y, chunk_position.z * world::CHUNK_SIZE_Z + block.position.z + 1 }, glm::vec3{ 0, -1, 0 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk_position.x * world::CHUNK_SIZE_X + block.position.x + 1, chunk_position.y * world::CHUNK_SIZE_Y + block.position.y, chunk_position.z * world::CHUNK_SIZE_Z + block.position.z }, glm::vec3{ 0, -1, 0 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk_position.x * world::CHUNK_SIZE_X + block.position.x + 1, chunk_position.y * world::CHUNK_SIZE_Y + block.position.y, chunk_position.z * world::CHUNK_SIZE_Z + block.position.z + 1 }, glm::vec3{ 0, -1, 0 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk_position.x * world::CHUNK_SIZE_X + block.position.x, chunk_position.y * world::CHUNK_SIZE_Y + block.position.y, chunk_position.z * world::CHUNK_SIZE_Z + block.position.z + 1 }, glm::vec3{ 0, -1, 0 } });
  //positive y
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk_position.x * world::CHUNK_SIZE_X + block.position.x, chunk_position.y * world::CHUNK_SIZE_Y + block.position.y + 1, chunk_position.z * world::CHUNK_SIZE_Z + block.position.z }, glm::vec3{ 0, 1, 0 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk_position.x * world::CHUNK_SIZE_X + block.position.x, chunk_position.y * world::CHUNK_SIZE_Y + block.position.y + 1, chunk_position.z * world::CHUNK_SIZE_Z + block.position.z + 1 }, glm::vec3{ 0, 1, 0 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk_position.x * world::CHUNK_SIZE_X + block.position.x + 1, chunk_position.y * world::CHUNK_SIZE_Y + block.position.y + 1, chunk_position.z * world::CHUNK_SIZE_Z + block.position.z }, glm::vec3{ 0, 1, 0 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk_position.x * world::CHUNK_SIZE_X + block.position.x + 1, chunk_position.y * world::CHUNK_SIZE_Y + block.position.y + 1, chunk_position.z * world::CHUNK_SIZE_Z + block.position.z }, glm::vec3{ 0, 1, 0 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk_position.x * world::CHUNK_SIZE_X + block.position.x, chunk_position.y * world::CHUNK_SIZE_Y + block.position.y + 1, chunk_position.z * world::CHUNK_SIZE_Z + block.position.z + 1 }, glm::vec3{ 0, 1, 0 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk_position.x * world::CHUNK_SIZE_X + block.position.x + 1, chunk_position.y * world::CHUNK_SIZE_Y + block.position.y + 1, chunk_position.z * world::CHUNK_SIZE_Z + block.position.z + 1 }, glm::vec3{ 0, 1, 0 } });
  //negative z
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk_position.x * world::CHUNK_SIZE_X + block.position.x, chunk_position.y * world::CHUNK_SIZE_Y + block.position.y, chunk_position.z * world::CHUNK_SIZE_Z + block.position.z }, glm::vec3{ 0, 0, -1 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk_position.x * world::CHUNK_SIZE_X + block.position.x, chunk_position.y * world::CHUNK_SIZE_Y + block.position.y + 1, chunk_position.z * world::CHUNK_SIZE_Z + block.position.z }, glm::vec3{ 0, 0, -1 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk_position.x * world::CHUNK_SIZE_X + block.position.x + 1, chunk_position.y * world::CHUNK_SIZE_Y + block.position.y, chunk_position.z * world::CHUNK_SIZE_Z + block.position.z }, glm::vec3{ 0, 0, -1 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk_position.x * world::CHUNK_SIZE_X + block.position.x, chunk_position.y * world::CHUNK_SIZE_Y + block.position.y + 1, chunk_position.z * world::CHUNK_SIZE_Z + block.position.z }, glm::vec3{ 0, 0, -1 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk_position.x * world::CHUNK_SIZE_X + block.position.x + 1, chunk_position.y * world::CHUNK_SIZE_Y + block.position.y + 1, chunk_position.z * world::CHUNK_SIZE_Z + block.position.z }, glm::vec3{ 0, 0, -1 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk_position.x * world::CHUNK_SIZE_X + block.position.x + 1, chunk_position.y * world::CHUNK_SIZE_Y + block.position.y, chunk_position.z * world::CHUNK_SIZE_Z + block.position.z }, glm::vec3{ 0, 0, -1 } });
  //positive z
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk_position.x * world::CHUNK_SIZE_X + block.position.x, chunk_position.y * world::CHUNK_SIZE_Y + block.position.y, chunk_position.z * world::CHUNK_SIZE_Z + block.position.z + 1 }, glm::vec3{ 0, 0, 1 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk_position.x * world::CHUNK_SIZE_X + block.position.x + 1, chunk_position.y * world::CHUNK_SIZE_Y + block.position.y, chunk_position.z * world::CHUNK_SIZE_Z + block.position.z + 1 }, glm::vec3{ 0, 0, 1 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk_position.x * world::CHUNK_SIZE_X + block.position.x, chunk_position.y * world::CHUNK_SIZE_Y + block.position.y + 1, chunk_position.z * world::CHUNK_SIZE_Z + block.position.z + 1 }, glm::vec3{ 0, 0, 1 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk_position.x * world::CHUNK_SIZE_X + block.position.x, chunk_position.y * world::CHUNK_SIZE_Y + block.position.y + 1, chunk_position.z * world::CHUNK_SIZE_Z + block.position.z + 1 }, glm::vec3{ 0, 0, 1 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk_position.x * world::CHUNK_SIZE_X + block.position.x + 1, chunk_position.y * world::CHUNK_SIZE_Y + block.position.y, chunk_position.z * world::CHUNK_SIZE_Z + block.position.z + 1 }, glm::vec3{ 0, 0, 1 } });
  block_vertex_data.emplace_back(renderer::Vertex{ glm::vec3{ chunk_position.x * world::CHUNK_SIZE_X + block.position.x + 1, chunk_position.y * world::CHUNK_SIZE_Y + block.position.y + 1, chunk_position.z * world::CHUNK_SIZE_Z + block.position.z + 1 }, glm::vec3{ 0, 0, 1 } });

  return block_vertex_data;
}
