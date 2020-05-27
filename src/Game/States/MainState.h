#pragma once
#include "glew_includes.h"
#include <Game/States/State.h>
#include <World/World.h>
#include <Shaders/DefaultShader.h>
#include <Renderer/ChunkRenderer.h>
#include <Camera.h>
#include <Texture/Atlas.h>

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
  std::unique_ptr<shaders::DefaultShader>  _shader;
  std::unique_ptr<textures::Atlas>         _atlas_texture;
  world::World                             _world;
};
}// namespace game::states
