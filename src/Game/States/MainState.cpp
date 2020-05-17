#include "MainState.h"
#include <glm/gtc/type_ptr.hpp>
#include <Renderer/ChunkMeshBuilder.h>

game::states::MainState::MainState(inputs::InputManager &input_manager, Window &window)
  : game::states::State(input_manager, window),
    _chunk_renderer(std::make_unique<renderer::ChunkRenderer>()),
    _camera(_input_manager, { 5, 10, 5 }),
    _world({}),
    _shader(std::make_unique<shaders::DefaultShader>(shaders::DefaultShader::create()))
{
}

void game::states::MainState::init()
{
  _world.build();

  for (const auto &chunk : _world.get_chunks_around(_camera.get_position(), 10)) {
    const auto &mesh = renderer::ChunkMeshBuilder::get_mesh(chunk);
    _chunk_renderer->update_mesh(chunk.id, mesh);
  }
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
}

void game::states::MainState::update(float elapsed_time)
{
  _camera.update(elapsed_time);
}

void game::states::MainState::render()
{
  _shader->bind();

  _shader->set_object_color(glm::vec3(0.11f, 0.87f, 0.10f));
  _shader->set_light_color(glm::vec3(0.5f * sinf(static_cast<float>(glfwGetTime())), 1.0f, 0.5f * cosf(static_cast<float>(glfwGetTime()))));
  _shader->set_light_position(glm::vec3(0.0f, 256.0f, 0.0f));
  _shader->set_view_position(_camera.get_position());
  _shader->set_view(_camera.get_view_matrix());
  _shader->set_projection(_window.get_screen_projection());
  _shader->set_model(glm::mat4{ 1.0f });

  _chunk_renderer->render();
  _shader->unbind();
}
