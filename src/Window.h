#pragma once

#include "glew_includes.h"
#include <GLFW/glfw3.h>
#include <memory>
#include <glm/mat4x4.hpp>

struct DestroyGlfwWindow
{
  void operator()(GLFWwindow *ptr) const noexcept
  {
    glfwDestroyWindow(ptr);
  }
};

class Window
{
public:
  Window(int width, int height);
  ~Window();

  [[nodiscard]] GLFWwindow *get_window_ptr() const noexcept;
  [[nodiscard]] GLFWwindow &get_window() const noexcept;
  [[nodiscard]] bool should_close() const noexcept;
  void swap_buffers() const noexcept;
  [[nodiscard]] glm::mat4 get_screen_projection() const noexcept;

private:
  int _width;
  int _height;
  std::unique_ptr<GLFWwindow, DestroyGlfwWindow> _window_ptr;
};
