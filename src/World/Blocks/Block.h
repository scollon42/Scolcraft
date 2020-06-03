#pragma once

#include <World/Blocks/BlockTextureCoordinates.h>

namespace world::blocks {

constexpr auto VISIBLE   = true;
constexpr auto INVISIBLE = false;


struct Block
{
  BlockTextureCoordinates texture_coordinates{};
  bool                    visible = INVISIBLE;
};
}// namespace world::blocks
