#include "Program.h"
#include <spdlog/spdlog.h>
#include <glad/glad.h>

shaders::Program::Program() : id(glCreateProgram())
{}

shaders::Program::~Program()
{
  glDeleteProgram(this->id);
  delete_attached_shaders();
}

void shaders::Program::attach_shader(shaders::shader_id shader_id) noexcept
{
  glAttachShader(this->id, shader_id);
  this->attached_shader_ids.emplace_back(shader_id);
}

void shaders::Program::link()
{
  glLinkProgram(this->id);

  int success = 0;
  glGetProgramiv(this->id, GL_LINK_STATUS, &success);

  if (!success) {
    char info[512];

    glGetProgramInfoLog(this->id, 512, nullptr, info);
    spdlog::error("Linking shader program failed : [{}]", info);
    throw std::runtime_error("Fail to link shaders");
  }

  delete_attached_shaders();
}

void shaders::Program::use() const noexcept
{
  glUseProgram(this->id);
}

void shaders::Program::delete_attached_shaders() noexcept
{
  for (const auto &shader_id : attached_shader_ids)
  {
    glDeleteShader(shader_id);
  }
  attached_shader_ids.clear();
}
