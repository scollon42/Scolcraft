#pragma once

#include "Chunk.h"

namespace world::chunks {

[[nodiscard]] Chunk generate_chunk(int id, const glm::vec2 &position);

}// namespace world::chunks
