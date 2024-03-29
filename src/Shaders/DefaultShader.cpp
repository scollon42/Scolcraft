#include "DefaultShader.h"
#include <Shaders/ShaderBuilder.h>
#include <spdlog/spdlog.h>

namespace uniforms {
constexpr auto MODEL_LOCATION          = "model";
constexpr auto VIEW_LOCATION           = "view";
constexpr auto PROJECTION_LOCATION     = "projection";
constexpr auto VIEW_POSITION_LOCATION  = "view_position";
constexpr auto LIGHT_COLOR_LOCATION    = "light_color";
constexpr auto LIGHT_POSITION_LOCATION = "light_position";
}// namespace uniforms

//TODO extract that in files
[[maybe_unused]] constexpr auto vertex_shader_src =
  R"(
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texture_coordinates;

out vec3 Fragment_position;
out vec3 Normal;
out vec2 Texture_coordinates;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    Fragment_position = vec3(model * vec4(position, 1.0));
    Normal = mat3(transpose(inverse(model))) * normal;
    Texture_coordinates = texture_coordinates;
}

)";

[[maybe_unused]] constexpr auto fragment_shader_src =
  R"(
#version 330 core
in vec3 Fragment_position;
in vec3 Normal;
in vec2 Texture_coordinates;

uniform sampler2D Texture;

out vec4 Fragment_color;

uniform vec3 light_position;
uniform vec3 view_position;
uniform vec3 light_color;

void main()
{
  vec4 texture_color =  texture(Texture, Texture_coordinates);

  vec3 norm = normalize(Normal);
  vec3 light_direction = normalize(light_position - Fragment_position);

  float ambient_strength = 0.2;
  vec3 ambient =  ambient_strength * texture_color.xyz;

  float diff = max(dot(norm, light_direction), 0.0);
  vec3 diffuse = diff * light_color;

  float shininess = 8;
  float specular_strength = 0.5;
  vec3 view_direction = normalize(view_position - Fragment_position);
  vec3 reflect_direction = reflect(-light_direction, norm);
  float spec = pow(max(dot(view_direction, reflect_direction), 0.0), shininess);
  vec3 specular = specular_strength * spec * light_color;

  vec3 result = (ambient + diffuse + specular ) * texture_color.xyz;
  Fragment_color = vec4(result, 1.0);
}
)";

shaders::DefaultShader *shaders::DefaultShader::create()
{
  return new shaders::DefaultShader{
    ShaderBuilder()
      .build()
      .add_vertex(vertex_shader_src)
      .add_fragment(fragment_shader_src)
      .link()
      .get()
  };
}

shaders::DefaultShader::~DefaultShader()
{
  spdlog::info("Destroying shader");
  destroy();
}

shaders::DefaultShader::DefaultShader(shaders::Shader &&shader)
  : Shader(shader)
{
}

void shaders::DefaultShader::set_model(const glm::mat4 &model) const noexcept
{
  set_uniform(uniforms::MODEL_LOCATION, model);
}

void shaders::DefaultShader::set_view(const glm::mat4 &view) const noexcept
{
  set_uniform(uniforms::VIEW_LOCATION, view);
}

void shaders::DefaultShader::set_projection(const glm::mat4 &model) const noexcept
{
  set_uniform(uniforms::PROJECTION_LOCATION, model);
}

void shaders::DefaultShader::set_view_position(const glm::vec3 &position) const noexcept
{
  set_uniform(uniforms::VIEW_POSITION_LOCATION, position);
}

void shaders::DefaultShader::set_light_color(const glm::vec3 &color) const noexcept
{
  set_uniform(uniforms::LIGHT_COLOR_LOCATION, color);
}

void shaders::DefaultShader::set_light_position(const glm::vec3 &position) const noexcept
{
  set_uniform(uniforms::LIGHT_POSITION_LOCATION, position);
}
