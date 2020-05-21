#include "InputManager.h"
#include <Events/KeyboardEvent.h>
#include <Events/MousePositionEvent.h>
#include <Events/Dispatcher.h>

void mouse_pos_callback([[maybe_unused]] GLFWwindow *window, double x_position, double y_position)
{
  //const auto input_manager = static_cast<inputs::InputManager *>(glfwGetWindowUserPointer(window));

  //input_manager->update_mouse_position(glm::vec2{
  //static_cast<float>(x_position), static_cast<float>(y_position) });

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
  //glfwSetWindowUserPointer(&_window, this);
  glfwSetCursorPosCallback(&_window, mouse_pos_callback);
  glfwSetKeyCallback(&_window, keyboard_callback);
}

//void inputs::InputManager::subscribe_to_mouse_event(const std::function<void(glm::vec2)> &callback) noexcept
//{
//_mouse_event_subscribers.push_back(callback);
//}

//void inputs::InputManager::update_mouse_position(const glm::vec2 &mouse_position) const noexcept
//{
//for (const auto &cb : _mouse_event_subscribers) {
//cb(mouse_position);
//}
//}

bool inputs::InputManager::is_pressed(int key_code) const
{
  return glfwGetKey(&_window, key_code) == GLFW_PRESS;
}
