#pragma once

#include <string>

namespace shaders {

enum class ShaderType { VERTEX, FRAGMENT };

unsigned int compile_shader_src(const std::string &src, shaders::ShaderType type);

};