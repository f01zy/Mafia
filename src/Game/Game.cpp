#include "Game.h"
#include "../Scenes/Scenes.h"

void Game::run() {
  Types::Scene scene = Types::Scene::Lobby;

  while (scene != Types::Scene::Exit) {
    switch (scene) {
    case Types::Scene::Lobby:
      scene = Scenes::Lobby();
      break;

    case Types::Scene::Login:
      scene = Scenes::Login();
      break;

    case Types::Scene::Register:
      scene = Scenes::Register();
      break;

    case Types::Scene::Exit:
      break;
    }
  }
}
