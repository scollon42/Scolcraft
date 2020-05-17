#pragma once

#include <memory>
#include <Renderer/ChunkRenderer.h>
#include <Window.h>
#include <Inputs/InputManager.h>
#include <Camera.h>
#include <World/World.h>
#include <Shaders/DefaultProgram.h>

class Game
{
public:
  Game();
  Game(const Game &) = delete;
  Game &operator=(const Game &) = delete;

  void init();

  void loop();

  void stop();

private:
  std::unique_ptr<Window> _window = nullptr;
  std::unique_ptr<inputs::InputManager> _input_manager = nullptr;
  std::unique_ptr<renderer::ChunkRenderer> _chunk_renderer = nullptr;
  Camera _camera;
  world::World _world;
  shaders::DefaultProgram _program;

  inline bool should_run() const noexcept
  {
    return !_window->should_close();
  }
};
