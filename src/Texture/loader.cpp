#include "loader.h"

#include <nlohmann/json.hpp>
#include <fstream>
#include <spdlog/spdlog.h>

using namespace textures;
using json = nlohmann::json;

[[maybe_unused]] std::unique_ptr<Atlas> loader::load_atlas(const std::string &json_path)
{
  spdlog::info("Loading texture definitions from [{}].", json_path);

  std::ifstream file;

  file.open(json_path);

  auto parsed = json::parse(file);

  file.close();

  const std::string terrain_file = parsed.value("terrain_file", "terrain.png");
  const int size = parsed.value("size", 0);
  const int texture_size = parsed.value("texture_size", 0);

  return std::make_unique<Atlas>(terrain_file, static_cast<size_t>(size), static_cast<size_t>(texture_size));
}
