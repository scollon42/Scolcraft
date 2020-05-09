#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <docopt/docopt.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <thread>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <Shaders/DefaultProgram.h>
#include <Utils/PerlinNoise.h>

#include <World/World.h>
#include <Renderer/ChunkRenderer.h>
#include <Camera.h>
#include <Inputs/InputManager.h>

struct DestroyGlfwWin
{
  void operator()(GLFWwindow *ptr) const noexcept
  {
    glfwDestroyWindow(ptr);
  }
};

void framebuffer_size_callback([[maybe_unused]] GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

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

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  std::unique_ptr<GLFWwindow, DestroyGlfwWin> window(glfwCreateWindow(1600, 1200, "OpenGL", nullptr, nullptr));

  if (window == nullptr) {
    spdlog::error("Failed to create GLFW window");
    std::abort();
  }

  glfwMakeContextCurrent(window.get());
  glewInit();

  glfwSetFramebufferSizeCallback(window.get(), framebuffer_size_callback);
  glfwSetInputMode(window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  auto input_manager = std::make_unique<inputs::InputManager>(*window);

  const auto program = shaders::DefaultProgram::build();

  auto chunk_renderer = std::make_unique<renderer::ChunkRenderer>();
  const auto world{ world::generate_world() };

  for (const auto &chunk : world.chunks) {
    const auto &mesh = world::get_chunk_mesh(chunk);
    chunk_renderer->update_mesh(chunk.id, mesh);
  }

  const auto &size = world.chunks.size();

  Camera camera{
    *input_manager,
    { size + world::CHUNK_SIZE_X / 2, 150, size + world::CHUNK_SIZE_Z / 2 }
  };

  float last_frame_time{ 0 };

  while (!glfwWindowShouldClose(window.get())) {
    const float current_frame_time{ static_cast<float>(glfwGetTime()) };
    const float time_elapsed = current_frame_time - last_frame_time;
    last_frame_time = current_frame_time;

    display_ms_per_frame(time_elapsed);

    camera.update(time_elapsed);

    if (input_manager->is_pressed(GLFW_KEY_ESCAPE)) {
      glfwSetWindowShouldClose(window.get(), true);
    }

    if (input_manager->is_pressed(GLFW_KEY_F)) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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

    // represent window screen
    glm::mat4 projection{ 1.0f };
    projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

    program.set_view_uniform(camera.get_view_matrix());
    program.set_projection_uniform(projection);

    glm::mat4 model{ 1.0f };
    program.set_model_uniform(model);

    chunk_renderer->render();

    glfwSwapBuffers(window.get());
    glfwPollEvents();
  }

  chunk_renderer = nullptr;

  glfwTerminate();

  return (0);
}
