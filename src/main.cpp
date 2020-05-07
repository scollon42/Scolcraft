#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <thread>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Shaders/DefaultProgram.h"
#include "Utils/PerlinNoise.h"

#include "World/World.h"
#include "Renderer/ChunkMeshBuilder.h"
#include "Renderer/ChunkRenderer.h"
struct DestroyGlfwWin
{
  void operator()(GLFWwindow *ptr) const noexcept
  {
    glfwDestroyWindow(ptr);
  }
};

unsigned int load_texture()
{
  unsigned int texture = 0;

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height, nb_channel;
  auto data = stbi_load("/home/scollon/Programming/best_cpp_project/container.jpg", &width, &height, &nb_channel, 0);

  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    spdlog::error("Failed to load texture");
    std::abort();
  }
  stbi_image_free(data);
  return texture;
}

void framebuffer_size_callback([[maybe_unused]] GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

float yaw{ 0 };
float pitch{ 0 };
glm::vec3 camera_front{ 0.0f, 0.0f, -1.0f };

void mouse_moved([[maybe_unused]] GLFWwindow *window, double position_x, double position_y) noexcept
{
  constexpr auto sensitivity{ 0.03f };

  static auto last_x_position = static_cast<float>(position_x);
  static auto last_y_position = static_cast<float>(position_y);

  spdlog::debug("Current mouse position is [{}, {}]", static_cast<float>(position_x), static_cast<float>(position_y));
  spdlog::debug("Last mouse position is [{}, {}]", last_x_position, last_y_position);

  const auto x_offset = (static_cast<float>(position_x) - last_x_position) * sensitivity;
  const auto y_offset = (last_y_position - static_cast<float>(position_y)) * sensitivity;//reverse to have natural mouse movement on y axis

  spdlog::debug("Calculated mouse offset is [{}, {}]", x_offset, y_offset);

  last_x_position = static_cast<float>(position_x);
  last_y_position = static_cast<float>(position_y);

  yaw += x_offset;
  pitch += y_offset;

  pitch = std::clamp(pitch, -89.0f, 89.0f);

  spdlog::debug("Yaw = [{}]; Pitch = [{}].", yaw, pitch);

  const glm::vec3 direction{
    cosf(glm::radians(yaw)) * cosf(glm::radians(pitch)),
    sinf(glm::radians(pitch)),
    sinf(glm::radians(yaw)) * cosf(glm::radians(pitch))
  };
  camera_front = glm::normalize(direction);
}

int main([[maybe_unused]] int argc, [[maybe_unused]] const char **argv)
{
  //  spdlog::set_level(spdlog::level::debug);
  spdlog::info("Starting game applicaton");
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  std::unique_ptr<GLFWwindow, DestroyGlfwWin> window{ glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr) };
  if (window.get() == nullptr) {
    spdlog::error("Failed to create GLFW window");
    std::abort();
  }

  glfwMakeContextCurrent(window.get());

  glfwSetFramebufferSizeCallback(window.get(), framebuffer_size_callback);
  glfwSetInputMode(window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window.get(), mouse_moved);

  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
    spdlog::error("Failed to initialize GLAD");
    std::abort();
  }


  const auto program = shaders::DefaultProgram::build();
  //  const auto texture = load_texture();

  auto chunk_renderer = renderer::ChunkRenderer{};

  //  const auto world{ world::World::generate_world(1) };

  //  for (const auto &chunk : world.get_chunks()) {
  auto chunk = world::Chunk::build_chunk(0, glm::vec3{ 0, 0, 0 });
  const auto mesh = renderer::ChunkMeshBuilder::build(chunk);
  //  mesh.build();
  //  }

  constexpr float base_camera_speed{ 3.0f };
  glm::vec3 camera_position{ 8, 2, 8 };
  const glm::vec3 camera_up{ 0.0f, 1.0f, 0.0f };

  float y_axis_position{ 256 };

  float last_frame_time{ 0 };

  while (!glfwWindowShouldClose(window.get())) {
    const float current_frame_time{ static_cast<float>(glfwGetTime()) };
    const float delta_time = current_frame_time - last_frame_time;
    last_frame_time = current_frame_time;
    const auto camera_speed = base_camera_speed * delta_time;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    if (glfwGetKey(window.get(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window.get(), true);
    }
    if (glfwGetKey(window.get(), GLFW_KEY_F) == GLFW_PRESS) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    if (glfwGetKey(window.get(), GLFW_KEY_L) == GLFW_PRESS) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if (glfwGetKey(window.get(), GLFW_KEY_W) == GLFW_PRESS) {
      camera_position += camera_speed * camera_front;
    }
    if (glfwGetKey(window.get(), GLFW_KEY_S) == GLFW_PRESS) {
      camera_position -= camera_speed * camera_front;
    }
    if (glfwGetKey(window.get(), GLFW_KEY_A) == GLFW_PRESS) {
      camera_position -= camera_speed * glm::normalize(glm::cross(camera_front, camera_up));
    }
    if (glfwGetKey(window.get(), GLFW_KEY_D) == GLFW_PRESS) {
      camera_position += camera_speed * glm::normalize(glm::cross(camera_front, camera_up));
    }
    if (glfwGetKey(window.get(), GLFW_KEY_SPACE) == GLFW_PRESS) {
      y_axis_position += 1.0f * camera_speed;
    }
    if (glfwGetKey(window.get(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
      y_axis_position -= 1.0f * camera_speed;
    }

    camera_position.y = y_axis_position;

    program.use();

    const auto &view_position_location = glGetUniformLocation(program.get_id(), "view_position");
    const auto &object_color_location = glGetUniformLocation(program.get_id(), "object_color");
    const auto &light_color_location = glGetUniformLocation(program.get_id(), "light_color");
    const auto &light_position_location = glGetUniformLocation(program.get_id(), "light_position");

    glUniform3fv(object_color_location, 1, glm::value_ptr(glm::vec3(0.35f, 0.87f, 0.10f)));
    glUniform3fv(light_color_location, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
    glUniform3fv(light_position_location, 1, glm::value_ptr(glm::vec3(15.0f, 50.0f, 10.0f)));
    glUniform3fv(view_position_location, 1, glm::value_ptr(camera_position));

    // represent camera
    glm::mat4 view{ 1.0f };
    view = glm::lookAt(camera_position, camera_position + camera_front, camera_up);

    glm::mat4 projection{ 1.0f };
    projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

    program.set_view_uniform(view);
    program.set_projection_uniform(projection);

    glm::mat4 model{ 1.0f };
    model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
    program.set_model_uniform(model);

    //    chunk_renderer.render();
    mesh.render();

    glfwSwapBuffers(window.get());

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glfwPollEvents();
  }

  return (0);
}
