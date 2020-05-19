#pragma once

#include <string>

namespace events {
class KeyboardEvent final
{
public:
  explicit KeyboardEvent(int key, int actions);

  [[maybe_unused]] [[nodiscard]] static std::string get_name() noexcept
  {
    return "KeyboardEvent";
  }

  [[maybe_unused]] [[nodiscard]] int get_key() const noexcept;
  [[maybe_unused]] [[nodiscard]] int get_action() const noexcept;

private:
  int _key;
  int _action;
};

}// namespace events
