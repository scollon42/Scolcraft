#include "Block.h"
#include <spdlog/spdlog.h>

bool world::is_visible_block_type(world::BlockType type) noexcept
{
  switch (type) {
  case BlockType::DIRT:
    return true;
  default:
    return false;
  }
}