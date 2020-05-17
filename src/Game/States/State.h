#pragma once

#include <Window.h>
#include <Inputs/InputManager.h>

namespace game::states {
class State
{
public:
  virtual ~State()                        = default;
  virtual void init()                     = 0;
  virtual void inputs()                   = 0;
  virtual void update(float elapsed_time) = 0;
  virtual void render()                   = 0;

  void start() noexcept
  {
    _is_running = true;
  }

  void stop() noexcept
  {
    _is_running = false;
  }

  [[nodiscard]] inline bool is_running() const noexcept
  {
    return _is_running;
  }

protected:
  explicit State(inputs::InputManager &input_manager, Window &window) : _input_manager(input_manager), _window(window)
  {
  }

  inputs::InputManager &_input_manager;
  Window &              _window;
  bool                  _is_running = false;
};
}// namespace game::states
