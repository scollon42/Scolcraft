#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <Inputs/InputManager.h>

namespace camera {
constexpr glm::vec3 CAMERA_UP{ glm::vec3(0.0, 1.0, 0.0) };
constexpr glm::vec3 CAMERA_FRONT{ glm::vec3(0.0f, 0.0f, -1.0f) };
}// namespace camera

class Camera final
{
public:
  Camera(const glm::vec3 &start_position);
  [[nodiscard]] glm::vec3 get_position() const noexcept;
  [[nodiscard]] glm::mat4 get_view_matrix() const noexcept;

  void update(float time_elapsed) noexcept;

private:
  // inputs::InputManager &_input_manager;
  glm::vec3 _position;
  glm::vec3 _front;
  float     _y_position;
  bool      _forward  = false;
  bool      _backward = false;
  bool      _left     = false;
  bool      _right    = false;
  bool      _up       = false;
  bool      _down     = false;
};
