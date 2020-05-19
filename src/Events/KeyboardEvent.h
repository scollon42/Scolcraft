#pragma once

#include <string>

namespace events {
struct KeyboardEvent final
{
  KeyboardEvent(int key, int actions);

  [[maybe_unused]] [[nodiscard]] static std::string get_name() noexcept
  {
    return "KeyboardEvent";
  }

  int key;
  int action;
};

}// namespace events
