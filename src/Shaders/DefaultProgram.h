#pragma once

#include <glm/glm.hpp>
#include <Shaders/Program.h>


namespace shaders {

class DefaultProgram : public Program
{
public:
  static DefaultProgram build();

  program_id get_id() const noexcept
  {
    return id;
  }

  void set_model_uniform(const glm::mat4 &model) const noexcept;
  void set_view_uniform(const glm::mat4 &model) const noexcept;
  void set_projection_uniform(const glm::mat4 &model) const noexcept;
};
}// namespace shaders
