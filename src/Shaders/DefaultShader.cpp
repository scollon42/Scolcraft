#include "DefaultShader.h"
#include <Shaders/ShaderBuilder.h>

namespace uniforms {
constexpr auto MODEL_LOCATION          = "model";
constexpr auto VIEW_LOCATION           = "view";
constexpr auto PROJECTION_LOCATION     = "projection";
constexpr auto VIEW_POSITION_LOCATION  = "view_position";
constexpr auto OBJECT_COLOR_LOCATION   = "object_color";
constexpr auto LIGHT_COLOR_LOCATION    = "light_color";
constexpr auto LIGHT_POSITION_LOCATION = "light_position";
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
    gl_Position = projection * view * model * vec4(position, 1.0);
    Fragment_position = vec3(model * vec4(position, 1.0));
    Normal = mat3(transpose(inverse(model))) * normal;
}

)";

[[maybe_unused]] constexpr auto fragment_shader_src =
  R"(
#version 330 core
in vec3 Fragment_position;
in vec3 Normal;

out vec4 Fragment_color;

uniform vec3 light_position;
uniform vec3 view_position;
uniform vec3 light_color;
uniform vec3 object_color;

uniform vec4 color;
void main()
{
  vec3 norm = normalize(Normal);
  vec3 light_direction = normalize(light_position - Fragment_position);

  float ambient_strength = 0.2;
  vec3 ambient =  ambient_strength * object_color;

  float diff = max(dot(norm, light_direction), 0.0);
  vec3 diffuse = diff * light_color;

  float shininess = 8;
  float specular_strength = 0.5;
  vec3 view_direction = normalize(view_position - Fragment_position);
  vec3 reflect_direction = reflect(-light_direction, norm);
  float spec = pow(max(dot(view_direction, reflect_direction), 0.0), shininess);
  vec3 specular = specular_strength * spec * light_color;

  vec3 result = (ambient + diffuse + specular ) * object_color;

  Fragment_color = vec4(result, 1.0);
}
)";

shaders::DefaultShader shaders::DefaultShader::create()
{
  return DefaultShader{
    ShaderBuilder()
      .add_vertex(vertex_shader_src)
      .add_fragment(fragment_shader_src)
      .link()
      .get()
  };
}

shaders::DefaultShader::DefaultShader(shaders::Shader &&shader)
  : Shader(shader)
{
}

void shaders::DefaultShader::set_model(const glm::mat4 &model) const noexcept
{
  set_uniform(uniforms::MODEL_LOCATION, model);
}

void shaders::DefaultShader::set_view(const glm::mat4 &model) const noexcept
{
  set_uniform(uniforms::VIEW_LOCATION, model);
}

void shaders::DefaultShader::set_projection(const glm::mat4 &model) const noexcept
{
  set_uniform(uniforms::PROJECTION_LOCATION, model);
}

void shaders::DefaultShader::set_view_position(const glm::vec3 &position) const noexcept
{
  set_uniform(uniforms::VIEW_POSITION_LOCATION, position);
}

void shaders::DefaultShader::set_object_color(const glm::vec3 &color) const noexcept
{
  set_uniform(uniforms::OBJECT_COLOR_LOCATION, color);
}

void shaders::DefaultShader::set_light_color(const glm::vec3 &color) const noexcept
{
  set_uniform(uniforms::LIGHT_COLOR_LOCATION, color);
}

void shaders::DefaultShader::set_light_position(const glm::vec3 &position) const noexcept
{
  set_uniform(uniforms::LIGHT_POSITION_LOCATION, position);
}
