#include "glew_includes.h"

#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>

shaders::Shader::~Shader()
{
  glDeleteProgram(_id);
}

void shaders::Shader::bind() const noexcept
{
  glUseProgram(_id);
}

void shaders::Shader::unbind() const noexcept
{
  glUseProgram(0);
}

unsigned int shaders::Shader::get_id() const noexcept
{
  return _id;
}

void shaders::Shader::set_uniform(const std::string &name, const glm::vec3 &v) const noexcept
{
  const auto location = glGetUniformLocation(_id, name.c_str());
  glUniform3fv(location, 1, glm::value_ptr(v));
}

void shaders::Shader::set_uniform(const std::string &name, const glm::mat4 &v) const noexcept
{
  const auto location = glGetUniformLocation(_id, name.c_str());
  glUniform4fv(location, 1, glm::value_ptr(v));
}
