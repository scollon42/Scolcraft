#pragma once

#include "Mesh.h"
#include "World/Chunk.h"

namespace renderer {
class ChunkMeshBuilder
{
public:
  static Mesh build(const world::Chunk &chunk) noexcept;
};
}// namespace renderer
