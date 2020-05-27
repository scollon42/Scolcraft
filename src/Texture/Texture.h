#pragma once

#include "glew_includes.h"
#include <string>

namespace textures {
class Texture
{
public:
  explicit Texture(const std::string &filename);

  ~Texture();

public:
  void bind() const noexcept;
  void unbind() const noexcept;

protected:
  int _width{};
  int _height{};
  int _channels_number{};

private:
  GLuint _id{};
};

}// namespace textures