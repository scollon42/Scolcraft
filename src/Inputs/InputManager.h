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

  //void subscribe_to_mouse_event(const std::function<void(glm::vec2)> &callback) noexcept;
  //TODO add unsubscribe

  //void update_mouse_position(const glm::vec2 &mouse_position) const noexcept;

  [[nodiscard]] bool is_pressed(int key_code) const;

private:
  GLFWwindow &_window;
  //std::vector<std::function<void(glm::vec2)>> _mouse_event_subscribers;
};
}// namespace inputs
