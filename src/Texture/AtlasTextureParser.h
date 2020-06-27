#ifndef GAME_ATLASTEXTUREPARSER_H
#define GAME_ATLASTEXTUREPARSER_H

#include <nlohmann/json.hpp>
#include <fstream>
#include <filesystem>
#include <spdlog/spdlog.h>
#include <World/Blocks/BlockTextureCoordinates.h>


using Json = nlohmann::json;

namespace textures
{

struct BlockData
{
  std::string type;
  world::blocks::BlockTextureCoordinates texture_coordinates;
};

struct TextureData
{
  std::string filename;
  std::filesystem::path filepath;
  std::size_t row_size{};
  std::size_t block_size{};
  std::vector<BlockData> block_data_list;
};


class AtlasTextureParser
{
public:
  [[nodiscard]] static TextureData parse(const std::filesystem::path &path)
  {
    const auto file_path = path / "texture.json";
    std::ifstream file(file_path.string());

    if (!file.is_open()) {
      spdlog::error("Failed to open texture file [{}].", file_path.string());
      std::abort();
    }

    Json json;

    file >> json;

    TextureData data;
    data.filename = json["filename"];
    data.filepath = path;
    spdlog::info("File is [{}]", (data.filepath / data.filename).string());
    data.row_size = json["row_size"].get<std::size_t>();
    spdlog::info("texture row size is {}", data.row_size);
    data.block_size = json["block_size"].get<std::size_t>();
    spdlog::info("texture block size is {}", data.block_size);

    for (auto & block : json["blocks"]) {
      const auto block_data = BlockData{
        block["type"].get<std::string>(),
        world::blocks::BlockTextureCoordinates{}
      };

      spdlog::info("Block {} is here", block_data.type);
      data.block_data_list.push_back(block_data);

      try {
        const auto a = block["textures"]["all"].get<std::array<int, 2>>();

        spdlog::info("test [{}, {}].", a[0], a[1]);
      } catch (std::exception &e) {

      }

      try {
        const auto a = block["textures"]["sides"].get<std::array<int, 2>>();

        spdlog::info("test [{}, {}].", a[0], a[1]);

        const auto b = block["textures"]["top"].get<std::array<int, 2>>();

        spdlog::info("test [{}, {}].", b[0], b[1]);

        const auto c = block["textures"]["bottom"].get<std::array<int, 2>>();

        spdlog::info("test [{}, {}].", c[0], c[1]);
      } catch (std::exception &e) {

      }


    }

    return data;
  }
};
}



#endif//GAME_ATLASTEXTUREPARSER_H
