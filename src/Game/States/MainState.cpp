#include "MainState.h"
#include <glm/gtc/type_ptr.hpp>
#include <Renderer/ChunkMeshBuilder.h>

game::states::MainState::MainState(inputs::InputManager &input_manager, Window &window)
  : game::states::State(input_manager, window),
    _chunk_renderer(std::make_unique<renderer::ChunkRenderer>()),
    _camera(_input_manager, { 5, 10, 5 }),
    _world({}),
    _program(shaders::DefaultProgram::build())
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
  _program.use();

  const auto &view_position_location  = glGetUniformLocation(_program.get_id(), "view_position");
  const auto &object_color_location   = glGetUniformLocation(_program.get_id(), "object_color");
  const auto &light_color_location    = glGetUniformLocation(_program.get_id(), "light_color");
  const auto &light_position_location = glGetUniformLocation(_program.get_id(), "light_position");

  glUniform3fv(object_color_location, 1, glm::value_ptr(glm::vec3(0.11f, 0.87f, 0.10f)));
  glUniform3fv(light_color_location, 1, glm::value_ptr(glm::vec3(0.5f * sinf(static_cast<float>(glfwGetTime())), 1.0f, 0.5f * cosf(static_cast<float>(glfwGetTime())))));
  glUniform3fv(light_position_location, 1, glm::value_ptr(glm::vec3(0.0f, 256.0f, 0.0f)));
  glUniform3fv(view_position_location, 1, glm::value_ptr(_camera.get_position()));

  _program.set_view_uniform(_camera.get_view_matrix());
  _program.set_projection_uniform(_window.get_screen_projection());

  glm::mat4 model{ 1.0f };
  _program.set_model_uniform(model);

  _chunk_renderer->render();
}
