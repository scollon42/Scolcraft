#include "InputManager.h"
#include <Events/KeyboardEvent.h>
#include <Events/MousePositionEvent.h>
#include <Events/Dispatcher.h>

void mouse_pos_callback([[maybe_unused]] GLFWwindow *window, double x_position, double y_position)
{
  events::MousePositionEvent event{ static_cast<float>(x_position), static_cast<float>(y_position) };
  events::Dispatcher::get().dispatch<events::MousePositionEvent>(event);
}

void keyboard_callback([[maybe_unused]] GLFWwindow *window, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods)
{
  events::KeyboardEvent event{ key, action };
  events::Dispatcher::get().dispatch<events::KeyboardEvent>(event);
}

inputs::InputManager::InputManager(GLFWwindow &window)
  : _window(window)
{
  glfwSetCursorPosCallback(&_window, mouse_pos_callback);
  glfwSetKeyCallback(&_window, keyboard_callback);
}

bool inputs::InputManager::is_pressed(int key_code) const
{
  return glfwGetKey(&_window, key_code) == GLFW_PRESS;
}
