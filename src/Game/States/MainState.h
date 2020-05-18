#pragma once
#include "glew_includes.h"
#include <Game/States/State.h>
#include <World/World.h>
#include <Shaders/DefaultShader.h>
#include <Renderer/ChunkRenderer.h>
#include <Camera.h>

namespace game::states {
class MainState final : public State
{
public:
  MainState(inputs::InputManager &input_manager, Window &window);

  void init() override;
  void inputs() override;
  void update(float elapsed_time) override;
  void render() override;

private:
  std::unique_ptr<renderer::ChunkRenderer> _chunk_renderer;
  Camera                                   _camera;
  world::World                             _world;
  std::unique_ptr<shaders::DefaultShader>  _shader;
};
}// namespace game::states
