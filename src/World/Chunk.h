#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>

#include "Block.h"
#include "Renderer/Mesh.h"

namespace world {

constexpr auto CHUNK_SIZE_X = 16;
constexpr auto CHUNK_SIZE_Y = 256;
constexpr auto CHUNK_SIZE_Z = 16;

using chunk_id = unsigned int;

class Chunk
{
public:
  static Chunk build_chunk(chunk_id id, const glm::vec3 &position);

  [[nodiscard]] chunk_id get_chunk_id() const noexcept;
  [[nodiscard]] const glm::vec3 &get_position() const noexcept;
  [[nodiscard]] const Block &get_block_at_absolute(const glm::vec3 &position) const;
  [[nodiscard]] const Block &get_block_at_relative(const glm::vec3 &position) const;
  [[nodiscard]] const std::vector<Block> &get_blocks() const noexcept;
  void add_block(Block &&block) noexcept;
  void add_block(const Block &block) noexcept;

protected:
  Chunk(chunk_id id, const glm::vec3 &position);
  Chunk(chunk_id id, const glm::vec3 &position, std::vector<Block> locks);

private:
  chunk_id _id;
  glm::vec3 _position;
  std::vector<Block> _blocks;
  renderer::Mesh _mesh;//FIXME not sure i want to keep it here
};


void build_mesh(Chunk &chunk);
void draw(const Chunk &chunk);
}// namespace world