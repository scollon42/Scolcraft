#pragma once

#include <memory>
#include <Window.h>
#include <Inputs/InputManager.h>
#include <Game/States/State.h>

namespace game {
class Game
{
public:
  Game();
  Game(const Game &) = delete;
  Game &operator=(const Game &) = delete;

  void init();

  void run();

  void stop();

private:
  std::size_t                                       _current_state_index;
  std::vector<std::unique_ptr<game::states::State>> _states;
  std::unique_ptr<Window>                           _window        = nullptr;
  std::unique_ptr<inputs::InputManager>             _input_manager = nullptr;

  [[nodiscard]] inline bool should_run() const noexcept
  {
    return !_window->should_close();
  }
};
}// namespace game
