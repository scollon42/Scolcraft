#include "Game.h"

#include <spdlog/spdlog.h>
#include <glm/gtc/type_ptr.hpp>
#include <Renderer/ChunkMeshBuilder.h>

inline void display_ms_per_frame(float time_elapsed) noexcept
{
  static unsigned int frame_count{ 0 };
  static auto accumulated_time{ 0.0f };

  accumulated_time += time_elapsed;
  frame_count++;
  if (accumulated_time >= 1.0f) {
    spdlog::info("Ms per frame [{} (~= {} fps)]", 1000.0 / static_cast<double>(frame_count), frame_count);

    frame_count = 0;
    accumulated_time = 1.0f - accumulated_time;
  }
}


Game::Game()
  : _window(std::make_unique<Window>(1200, 800, "Our Game")),
    _input_manager(std::make_unique<inputs::InputManager>(_window->get_window())),
    _chunk_renderer(std::make_unique<renderer::ChunkRenderer>()),
    _camera(*_input_manager, { 5, 10, 5 }),
    _world({}),
    _program(shaders::DefaultProgram::build())
{
}

void Game::init()
{
  spdlog::info("Initiliazing game");

  _world.build();

  for (const auto &chunk : _world.get_chunks_around(_camera.get_position(), 10)) {
    const auto &mesh = renderer::ChunkMeshBuilder::get_mesh(chunk);
    _chunk_renderer->update_mesh(chunk.id, mesh);
  }
}

void Game::loop()
{
  spdlog::info("Starting main loop");
  float last_frame_time{ 0 };

  while (should_run()) {
    const float current_frame_time{ static_cast<float>(glfwGetTime()) };
    const float time_elapsed = current_frame_time - last_frame_time;
    last_frame_time = current_frame_time;

    display_ms_per_frame(time_elapsed);

    inputs();
    update(time_elapsed);
    render();

    _window->swap_buffers();
    glfwPollEvents();
  }
}

void Game::inputs()
{
  if (_input_manager->is_pressed(GLFW_KEY_ESCAPE)) {
    glfwSetWindowShouldClose(_window->get_window_ptr(), true);
  }

  if (_input_manager->is_pressed(GLFW_KEY_L)) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }

  if (_input_manager->is_pressed(GLFW_KEY_F)) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
}

void Game::update(const float time_elapsed)
{
  _camera.update(time_elapsed);
}

void Game::render()
{
  _program.use();

  const auto &view_position_location = glGetUniformLocation(_program.get_id(), "view_position");
  const auto &object_color_location = glGetUniformLocation(_program.get_id(), "object_color");
  const auto &light_color_location = glGetUniformLocation(_program.get_id(), "light_color");
  const auto &light_position_location = glGetUniformLocation(_program.get_id(), "light_position");

  glUniform3fv(object_color_location, 1, glm::value_ptr(glm::vec3(0.11f, 0.87f, 0.10f)));
  glUniform3fv(light_color_location, 1, glm::value_ptr(glm::vec3(0.5f * sinf(static_cast<float>(glfwGetTime())), 1.0f, 0.5f * cosf(static_cast<float>(glfwGetTime())))));
  glUniform3fv(light_position_location, 1, glm::value_ptr(glm::vec3(0.0f, 256.0f, 0.0f)));
  glUniform3fv(view_position_location, 1, glm::value_ptr(_camera.get_position()));

  _program.set_view_uniform(_camera.get_view_matrix());
  _program.set_projection_uniform(_window->get_screen_projection());

  glm::mat4 model{ 1.0f };
  _program.set_model_uniform(model);

  _chunk_renderer->render();
}


void Game::stop()
{
  spdlog::info("Stopping game");
  _chunk_renderer = nullptr;

  glfwTerminate();
}
