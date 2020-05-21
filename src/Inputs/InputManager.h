#pragma once

#include <GLFW/glfw3.h>
#include <memory>
#include <functional>
#include <glm/vec2.hpp>

namespace inputs {

class InputManager final
{
public:
  explicit InputManager(GLFWwindow &window);

  [[nodiscard]] bool is_pressed(int key_code) const;

private:
  GLFWwindow &_window;
};
}// namespace inputs
