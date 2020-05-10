#include "glew_includes.h"
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <thread>
#include <glm/gtc/type_ptr.hpp>
#include <Shaders/DefaultProgram.h>
#include <World/World.h>
#include <Renderer/ChunkRenderer.h>
#include <Camera.h>
#include <Inputs/InputManager.h>
#include <Window.h>

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

int main([[maybe_unused]] int argc, [[maybe_unused]] const char **argv)
{
  spdlog::info("Starting game applicaton");

  auto window = std::make_unique<Window>(1200, 800);

  auto input_manager = std::make_unique<inputs::InputManager>(window->get_window());

  const auto program = shaders::DefaultProgram::build();

  auto chunk_renderer = std::make_unique<renderer::ChunkRenderer>();

  Camera camera{
    *input_manager,
    { 5, 10, 5 }
  };

  world::World world{};

  world.build();

  float last_frame_time{ 0 };
  auto last_camera_position = glm::vec3{ 0.0f };
  while (!window->should_close()) {
    const float current_frame_time{ static_cast<float>(glfwGetTime()) };
    const float time_elapsed = current_frame_time - last_frame_time;
    last_frame_time = current_frame_time;

    display_ms_per_frame(time_elapsed);

    camera.update(time_elapsed);


    if (input_manager->is_pressed(GLFW_KEY_ESCAPE)) {
      glfwSetWindowShouldClose(window->get_window_ptr(), true);
    }

    if (input_manager->is_pressed(GLFW_KEY_L)) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    if (input_manager->is_pressed(GLFW_KEY_F)) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }


    if (last_camera_position != camera.get_position()) {
      //      chunk_renderer->clear_mesh();
      for (const auto &chunk : world.get_chunks_around(camera.get_position(), 1)) {
        const auto &mesh = world::get_chunk_mesh(chunk);
        chunk_renderer->update_mesh(chunk.id, mesh);
      }

      last_camera_position = camera.get_position();
    }


    program.use();

    const auto &view_position_location = glGetUniformLocation(program.get_id(), "view_position");
    const auto &object_color_location = glGetUniformLocation(program.get_id(), "object_color");
    const auto &light_color_location = glGetUniformLocation(program.get_id(), "light_color");
    const auto &light_position_location = glGetUniformLocation(program.get_id(), "light_position");

    glUniform3fv(object_color_location, 1, glm::value_ptr(glm::vec3(0.11f, 0.87f, 0.10f)));
    glUniform3fv(light_color_location, 1, glm::value_ptr(glm::vec3(0.5f * sinf(static_cast<float>(glfwGetTime())), 1.0f, 0.5f * cosf(static_cast<float>(glfwGetTime())))));
    glUniform3fv(light_position_location, 1, glm::value_ptr(glm::vec3(0.0f, 256.0f, 0.0f)));
    glUniform3fv(view_position_location, 1, glm::value_ptr(camera.get_position()));

    program.set_view_uniform(camera.get_view_matrix());
    program.set_projection_uniform(window->get_screen_projection());

    glm::mat4 model{ 1.0f };
    program.set_model_uniform(model);

    chunk_renderer->render();

    window->swap_buffers();
    glfwPollEvents();
  }

  chunk_renderer = nullptr;

  glfwTerminate();

  return (0);
}
