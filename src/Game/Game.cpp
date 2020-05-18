#include "Game.h"

#include <spdlog/spdlog.h>
#include <Game/States/MainState.h>

inline void display_ms_per_frame(float time_elapsed) noexcept
{
  static unsigned int frame_count{ 0 };
  static auto         accumulated_time{ 0.0f };

  accumulated_time += time_elapsed;
  frame_count++;
  if (accumulated_time >= 1.0f) {
    spdlog::info("Ms per frame [{} (~= {} fps)]", 1000.0 / static_cast<double>(frame_count), frame_count);

    frame_count      = 0;
    accumulated_time = 1.0f - accumulated_time;
  }
}

game::Game::Game()
  : _current_state_index(0),
    _window(std::make_unique<Window>(1200, 800, "Our Game")),
    _input_manager(std::make_unique<inputs::InputManager>(_window->get_window()))
{
}

void game::Game::init()
{
  spdlog::info("Initializing game");
  _states.push_back(std::make_unique<game::states::MainState>(*_input_manager, *_window));
  _current_state_index = 0;
}

void game::Game::run()
{
  spdlog::info("Starting main run");
  float last_frame_time{ 0 };

  game::states::State &current_state{ *_states.at(_current_state_index) };

  while (should_run()) {

    current_state.start();
    current_state.init();

    while (current_state.is_running()) {

      const float current_frame_time{ static_cast<float>(glfwGetTime()) };
      const float time_elapsed = current_frame_time - last_frame_time;
      last_frame_time          = current_frame_time;

      display_ms_per_frame(time_elapsed);

      current_state.inputs();
      current_state.update(time_elapsed);
      current_state.render();

      _window->swap_buffers();
      glfwPollEvents();
    }
    // TODO : check if another state should be run after
    glfwSetWindowShouldClose(_window->get_window_ptr(), true);
  }
}

void game::Game::stop()
{
  spdlog::info("Stopping game");

  glfwTerminate();
}
