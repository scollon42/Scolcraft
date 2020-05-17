#include "InputManager.h"

void mouse_pos_callback(GLFWwindow *window, double x_position, double y_position)
{
  const auto input_manager = static_cast<inputs::InputManager *>(glfwGetWindowUserPointer(window));

  input_manager->update_mouse_position(glm::vec2{
    static_cast<float>(x_position), static_cast<float>(y_position) });
}

inputs::InputManager::InputManager(GLFWwindow &window)
  : _window(window)
{
  glfwSetWindowUserPointer(&_window, this);
  glfwSetCursorPosCallback(&_window, mouse_pos_callback);
}

void inputs::InputManager::subscribe_to_mouse_event(const std::function<void(glm::vec2)> &callback) noexcept
{
  _mouse_event_subscribers.push_back(callback);
}

void inputs::InputManager::update_mouse_position(const glm::vec2 &mouse_position) const noexcept
{
  for (const auto &cb : _mouse_event_subscribers) {
    cb(mouse_position);
  }
}

bool inputs::InputManager::is_pressed(int key_code) const
{
  return glfwGetKey(&_window, key_code) == GLFW_PRESS;
}
