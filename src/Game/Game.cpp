#include "Game.h"
#include "../Scenes/Scenes.h"
#include "../Utils/Auth.h"

void Game::run() {
  bool isAuth = Utils::Auth::checkAuth();
  Types::Scene scene = isAuth ? Types::Scene::Lobby : Types::Scene::Login;

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
    }
  }
}
