#pragma once

#include <Texture/Texture.h>
#include <memory>
#include <vector>
#include <glm/vec2.hpp>

namespace textures {
class Atlas final : public Texture
{
public:
  Atlas(const std::string &texture_filename, std::size_t size, std::size_t texture_size);

  [[nodiscard]] std::array<glm::vec2, 6> get_texture_coordinates(const glm::vec2 &coordinate) const noexcept;

private:
  std::size_t _size;
  std::size_t _texture_size;
};
}// namespace textures
