#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <spdlog/spdlog.h>
#include <World/World.h>

constexpr auto CAMERA_BASE_SPEED = 10.0f;
constexpr auto CAMERA_SENSITIVITY{ 0.03f };

Camera::Camera(inputs::InputManager &input_manager, const glm::vec3 &start_position)
  : _input_manager(input_manager),
    _position(start_position),
    _front(camera::CAMERA_FRONT),
    _y_position(_position.y)
{
  _input_manager.subscribe_to_mouse_event([&](const glm::vec2 &mouse_position) {
    static float yaw{ 0 };
    static float pitch{ 0 };
    static auto  last_x_position{ mouse_position.x };
    static auto  last_y_position{ mouse_position.y };

    const auto x_offset = (mouse_position.x - last_x_position) * CAMERA_SENSITIVITY;
    const auto y_offset = (last_y_position - mouse_position.y) * CAMERA_SENSITIVITY;//reverse to have natural mouse movement on y axis

    last_x_position = mouse_position.x;
    last_y_position = mouse_position.y;

    yaw += x_offset;
    pitch += y_offset;

    pitch = std::clamp(pitch, -89.0f, 89.0f);
    const glm::vec3 direction{
      cosf(glm::radians(yaw)) * cosf(glm::radians(pitch)),
      sinf(glm::radians(pitch)),
      sinf(glm::radians(yaw)) * cosf(glm::radians(pitch))
    };

    _front = glm::normalize(direction);// set front
  });
}

glm::vec3 Camera::get_position() const noexcept
{
  return _position;
}

glm::mat4 Camera::get_view_matrix() const noexcept
{
  return glm::lookAt(_position, _position + _front, camera::CAMERA_UP);
}

void Camera::update(float time_elapsed) noexcept
{
  const auto speed = CAMERA_BASE_SPEED * static_cast<float>(time_elapsed);

  if (_input_manager.is_pressed(GLFW_KEY_W)) {
    _position += speed * _front;
  }

  if (_input_manager.is_pressed(GLFW_KEY_S)) {
    _position -= speed * _front;
  }

  if (_input_manager.is_pressed(GLFW_KEY_A)) {
    _position -= speed * glm::normalize(glm::cross(_front, camera::CAMERA_UP));
  }

  if (_input_manager.is_pressed(GLFW_KEY_D)) {
    _position += speed * glm::normalize(glm::cross(_front, camera::CAMERA_UP));
  }

  if (_input_manager.is_pressed(GLFW_KEY_SPACE)) {
    _y_position += 1.0f * speed;
  }

  if (_input_manager.is_pressed(GLFW_KEY_LEFT_SHIFT)) {
    _y_position -= 1.0f * speed;
  }

  _position.y = _y_position;
}
