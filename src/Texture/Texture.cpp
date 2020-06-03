#include "Texture.h"
#include <spdlog/spdlog.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

textures::Texture::Texture(const std::string &filename)
{
  spdlog::info("Creating Texture Object.");

  int channels_number{ 0 };

  const auto data = stbi_load(filename.c_str(), &_width, &_height, &channels_number, 0);

  if (data == nullptr) {
    throw std::runtime_error(fmt::format("Failed to load texture [{}]", filename));
  }

  glGenTextures(1, &_id);
  bind();

  const GLenum channel_mod = channels_number == 4 ? GL_RGBA : GL_RGB;

  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    static_cast<int>(channel_mod),
    _width,
    _height,
    0,
    channel_mod,
    GL_UNSIGNED_BYTE,
    data);

  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glBindTexture(GL_TEXTURE_2D, 0);
  stbi_image_free(data);
  unbind();
}

textures::Texture::~Texture()
{
  spdlog::info("Destroying Texture Object.");
  glDeleteTextures(1, &_id);
}
void textures::Texture::bind() const noexcept
{
  glBindTexture(GL_TEXTURE_2D, _id);
}

void textures::Texture::unbind() const noexcept
{
  glBindTexture(GL_TEXTURE_2D, 0);
}
