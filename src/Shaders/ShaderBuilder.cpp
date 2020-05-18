#include "glew_includes.h"

#include "ShaderBuilder.h"
#include <spdlog/spdlog.h>

enum class ShaderType {
  VERTEX,
  FRAGMENT
};

[[nodiscard]] unsigned int compile_shader_src(const std::string &src, ShaderType type);


shaders::ShaderBuilder &shaders::ShaderBuilder::build() noexcept
{
  _shader.set_id(glCreateProgram());
  return *this;
}

shaders::ShaderBuilder &shaders::ShaderBuilder::add_vertex(const std::string &vertex_src)
{
  const auto shader_id = compile_shader_src(vertex_src, ShaderType::VERTEX);
  glAttachShader(_shader.get_id(), shader_id);
  _attached_shader_ids.push_back(shader_id);
  return *this;
}
shaders::ShaderBuilder &shaders::ShaderBuilder::add_fragment(const std::string &fragment_str)
{
  const auto shader_id = compile_shader_src(fragment_str, ShaderType::FRAGMENT);
  glAttachShader(_shader.get_id(), shader_id);
  _attached_shader_ids.push_back(shader_id);
  return *this;
}
const shaders::ShaderBuilder &shaders::ShaderBuilder::link()
{
  glLinkProgram(_shader.get_id());

  int success = 0;
  glGetProgramiv(_shader.get_id(), GL_LINK_STATUS, &success);

  if (!success) {
    char info[512];

    glGetProgramInfoLog(_shader.get_id(), 512, nullptr, info);
    spdlog::error("Linking shader program failed : [{}]", info);
    throw std::runtime_error("Fail to link shaders");
  } else {
    spdlog::info("Program {} was linked successfully", _shader.get_id());
  }

  for (const auto &id : _attached_shader_ids) {
    glDeleteShader(id);
  }

  _attached_shader_ids.clear();

  return *this;
}

shaders::Shader shaders::ShaderBuilder::get() const noexcept
{
  return _shader;
}

unsigned int compile_shader_src(const std::string &src, ShaderType type)
{
  int          success = 0;
  unsigned int id;
  const char * c_src = src.c_str();

  id = glCreateShader(type == ShaderType::VERTEX ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
  glShaderSource(id, 1, &c_src, nullptr);
  glCompileShader(id);
  glGetShaderiv(id, GL_COMPILE_STATUS, &success);

  if (!success) {
    char info[512];

    glGetShaderInfoLog(id, 512, nullptr, info);
    spdlog::error("Compiling shader failed : [{}]", info);
    throw std::runtime_error(fmt::format("Fail to compile {} shader.", type == ShaderType::VERTEX ? "Vertex" : "Fragment"));
  }

  return id;
}
