#include <spdlog/spdlog.h>
#include <Game/Game.h>

int main([[maybe_unused]] int argc, [[maybe_unused]] const char **argv)
{
  auto game = std::make_unique<game::Game>();

  game->init();
  game->run();
  game->stop();

  return (0);
}
