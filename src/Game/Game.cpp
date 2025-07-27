#include "Game.h"
#include "../Network/Socket.h"
#include "../Scenes/Scenes.h"
#include "../Utils/Auth.h"

void Game::run() {
  bool isAuth = Utils::Auth::checkAuth();
  Types::Scene scene = isAuth ? Types::Scene::Menu : Types::Scene::Login;

  Socket &socket = Socket::getInstance();
  if (isAuth) {
    socket.auth();
  }

  while (scene != Types::Scene::Exit) {
    switch (scene) {
    case Types::Scene::Menu:
      scene = Scenes::Menu();
      break;

    case Types::Scene::Rooms:
      scene = Scenes::Rooms();
      break;

    case Types::Scene::CreateRoom:
      scene = Scenes::CreateRoom();
      break;

    case Types::Scene::Room:
      scene = Scenes::Room();
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
