#pragma once

#include "Atlas.h"

namespace textures::loader {

[[maybe_unused]] [[nodiscard]] std::unique_ptr<Atlas> load_atlas(const std::string &json_path);

}// namespace textures::loader
