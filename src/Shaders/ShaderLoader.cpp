#include "ShaderLoader.h"

#include <GL/glew.h>
#include <spdlog/spdlog.h>

// TODO refactor this code
unsigned int shaders::compile_shader_src(const std::string &src, shaders::ShaderType type)
{
  int success = 0;
  unsigned int id;
  const char *c_src = src.c_str();

  id = glCreateShader(type == shaders::ShaderType::VERTEX ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
  glShaderSource(id, 1, &c_src, nullptr);
  glCompileShader(id);
  glGetShaderiv(id, GL_COMPILE_STATUS, &success);

  if (!success) {
    char info[512];

    glGetShaderInfoLog(id, 512, nullptr, info);
    spdlog::error("Compiling shader failed : [{}]", info);
    throw std::runtime_error(fmt::format("Fail to compile {} shader.", type == shaders::ShaderType::VERTEX ? "Vertex" : "Fragment"));
  }

  return id;
}
