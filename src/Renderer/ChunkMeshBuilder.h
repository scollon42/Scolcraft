#pragma once

#include <Renderer/Mesh.h>
#include <Renderer/Vertex.h>
#include <World/Chunk.h>

namespace renderer {
namespace ChunkMeshBuilder {
  [[nodiscard]] renderer::Mesh get_mesh(const world::Chunk &chunk) noexcept;
};// namespace ChunkMeshBuilder
}// namespace renderer
