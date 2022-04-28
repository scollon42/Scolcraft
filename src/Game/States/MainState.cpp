#include "MainState.h"
#include <glm/gtc/type_ptr.hpp>
#include <Renderer/ChunkMeshBuilder.h>
#include <Utils/Profiler.h>
#include "Texture/loader.h"

game::states::MainState::MainState(inputs::InputManager &input_manager, Window &window)
  : game::states::State(input_manager, window),
    _camera({ 5, 10, 5 }),
    _shader(shaders::DefaultShader::create()),
    _world({})
{
  _world.init();
  _atlas_texture = textures::loader::load_atlas("/Users/scollon/Projects/Scolcraft/texture.json");
  _world_renderer = std::make_unique<renderer::WorldRenderer>(_world, *_atlas_texture);
}

void game::states::MainState::init()
{
  spdlog::info("MainState : [INIT].");
}

void game::states::MainState::inputs()
{
  if (_input_manager.is_pressed(GLFW_KEY_ESCAPE)) {
    _is_running = false;
  }

  if (_input_manager.is_pressed(GLFW_KEY_L)) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }

  if (_input_manager.is_pressed(GLFW_KEY_F)) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  if (_input_manager.is_pressed(GLFW_KEY_C)) {
    glfwSetInputMode(_window.get_window_ptr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }

  if (_input_manager.is_pressed(GLFW_KEY_U)) {
    glfwSetInputMode(_window.get_window_ptr(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }
}

void game::states::MainState::update(float elapsed_time)
{
  _camera.update(elapsed_time);
  _world_renderer->update(_world, _camera);
}


void game::states::MainState::render()
{
  _shader->bind();
  _atlas_texture->bind();
  _shader->set_light_color(glm::vec3(1.0f, 1.0f, 1.0f));
  _shader->set_light_position(glm::vec3(2000.0f, 2000.0f, 0.0f));
  _shader->set_view_position(_camera.get_position());
  _shader->set_view(_camera.get_view_matrix());
  _shader->set_projection(_window.get_screen_projection());

  // FIXME: Use model inside of chunk to translate chunk position
  _shader->set_model(glm::mat4{ 1.0f });

  _world_renderer->render();

  _atlas_texture->unbind();
  _shader->unbind();
}
