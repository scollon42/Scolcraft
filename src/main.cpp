#include "glew_includes.h"
#include <spdlog/spdlog.h>
#include <Game.h>


int main([[maybe_unused]] int argc, [[maybe_unused]] const char **argv)
{
  auto game = std::make_unique<Game>();

  game->init();
  game->loop();
  game->stop();

  return (0);
}
