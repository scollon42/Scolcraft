#pragma once

#include <vector>
#include "Chunk.h"

namespace world {
struct World
{
  std::vector<Chunk> chunks;
};

[[nodiscard]] World generate_world() noexcept;

}// namespace world
