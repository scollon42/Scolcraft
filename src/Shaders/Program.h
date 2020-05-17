#pragma once

#include <vector>
#include <string>

namespace shaders {
using shader_id  = unsigned int;
using program_id = unsigned int;

class Program
{
public:
  Program();
  ~Program();
  void use() const noexcept;

protected:
  program_id id;

  void attach_shader(shader_id id) noexcept;
  void link();

private:
  std::vector<shader_id> attached_shader_ids;

  void delete_attached_shaders() noexcept;
};
}// namespace shaders