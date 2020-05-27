#include "Window.h"

#include <spdlog/spdlog.h>
#include <glm/gtc/matrix_transform.hpp>

void framebuffer_size_callback([[maybe_unused]] GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}


Window::Window(int width, int height, const std::string &title)
  : _width(width), _height(height), _window_ptr(nullptr)
{
  spdlog::info("Creating window.");

  if (glfwInit() == GLFW_FALSE) {
    spdlog::error("Failed to initialize GLFW");
    std::abort();
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  _window_ptr.reset(glfwCreateWindow(_width, _height, title.data(), nullptr, nullptr));

  if (_window_ptr == nullptr) {
    spdlog::error("Failed to create GLFW window");
    std::abort();
  }

  glfwMakeContextCurrent(_window_ptr.get());

  glewInit();

  glfwSetFramebufferSizeCallback(_window_ptr.get(), framebuffer_size_callback);
  //  glfwSetInputMode(_window_ptr.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

Window::~Window()
{
  spdlog::info("Destroying window.");
  glfwDestroyWindow(_window_ptr.get());
}

GLFWwindow *Window::get_window_ptr() const noexcept
{
  return _window_ptr.get();
}

GLFWwindow &Window::get_window() const noexcept
{
  return *_window_ptr;
}

bool Window::should_close() const noexcept
{
  return glfwWindowShouldClose(_window_ptr.get());
}

void Window::swap_buffers() const noexcept
{
  glfwSwapBuffers(_window_ptr.get());
}

glm::mat4 Window::get_screen_projection() const noexcept
{
  return glm::perspective(glm::radians(45.0f), static_cast<float>(_width) / static_cast<float>(_height), 0.1f, 200.0f);
}
