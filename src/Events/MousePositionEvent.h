#pragma once

#include <string>

namespace events {
struct MousePositionEvent
{
  MousePositionEvent(float x, float y);

  [[maybe_unused]] [[nodiscard]] static std::string get_name() noexcept
  {
    return "MousePositionEvent";
  }

  float x;
  float y;
};
}// namespace events
