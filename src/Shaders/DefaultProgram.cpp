#include "DefaultProgram.h"
#include <Shaders/Program.h>
#include <Shaders/ShaderLoader.h>

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <string>

namespace uniforms {
constexpr auto MODEL_LOCATION = "model";
constexpr auto VIEW_LOCATION = "view";
constexpr auto PROJECTION_LOCATION = "projection";
}// namespace uniforms


//TODO extract that in files
[[maybe_unused]] constexpr auto vertex_shader_src =
  R"(
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 Fragment_position;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    Fragment_position = vec3(model * vec4(position, 1.0));
    Normal = mat3(transpose(inverse(model))) * normal;
    gl_Position = projection * view * vec4(Fragment_position, 1.0);
    //vertex_text_coord = aTextCoord;
}

)";

[[maybe_unused]] constexpr auto fragment_shader_src =
  R"(
#version 330 core
out vec4 Fragment_color;

//in vec2 vertex_text_coord;
in vec3 Fragment_position;
in vec3 Normal;
//uniform sampler2D u_texture;
uniform vec3 light_position;
uniform vec3 view_position;
uniform vec3 light_color;
uniform vec3 object_color;

uniform vec4 color;
void main()
{
  float ambient_strength = 0.1;
  vec3 ambient = object_color * ambient_strength;

  vec3 normal = normalize(Normal);

  vec3 light_direction = normalize(light_position - Fragment_position);
  float diff = max(dot(normal, light_direction), 0.0);
  vec3 diffuse = diff * light_color;

  float specular_strength = 0.5;
  vec3 view_direction = normalize(view_position - Fragment_position);
  vec3 reflect_direction = reflect(-light_direction, normal);
  float spec = pow(max(dot(view_direction, reflect_direction), 0.0), 32);
  vec3 specular = specular_strength * spec * light_color;

  vec3 result = (ambient + diffuse + specular) * object_color;

  Fragment_color = vec4(result, 1.0);
}

)";
// TODO refactor
void set_uniform(shaders::program_id program_id, const std::string &location, const glm::mat4 &uniform)
{
  const auto location_ptr = glGetUniformLocation(program_id, location.c_str());
  glUniformMatrix4fv(location_ptr, 1, GL_FALSE, glm::value_ptr(uniform));
}

shaders::DefaultProgram shaders::DefaultProgram::build()
{
  shaders::DefaultProgram program{};

  const auto &vertex_shader_id = shaders::compile_shader_src(vertex_shader_src, shaders::ShaderType::VERTEX);
  const auto &fragment_shader_id = shaders::compile_shader_src(fragment_shader_src, shaders::ShaderType::FRAGMENT);

  program.attach_shader(vertex_shader_id);
  program.attach_shader(fragment_shader_id);

  program.link();

  return program;
}

void shaders::DefaultProgram::set_model_uniform(const glm::mat4 &model) const noexcept
{
  set_uniform(this->id, uniforms::MODEL_LOCATION, model);
}

void shaders::DefaultProgram::set_view_uniform(const glm::mat4 &model) const noexcept
{
  set_uniform(this->id, uniforms::VIEW_LOCATION, model);
}
void shaders::DefaultProgram::set_projection_uniform(const glm::mat4 &model) const noexcept
{
  set_uniform(this->id, uniforms::PROJECTION_LOCATION, model);
}