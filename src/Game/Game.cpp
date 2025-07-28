#include "Game.h"
#include "../Network/Socket.h"
#include "../Scenes/Scenes.h"
#include "../Utils/Auth.h"
#include <unordered_map>

void Game::run() {
  bool isAuth = Utils::Auth::checkAuth();
  Types::Scene scene = isAuth ? Types::Scene::Menu : Types::Scene::Login;

  Socket &socket = Socket::getInstance();
  if (isAuth) {
    socket.auth();
  }

  std::unordered_map<Types::Scene, std::function<Types::Scene()>> scenes = {
      {Types::Scene::Menu, []() { return Scenes::Menu(); }},
      {Types::Scene::Rooms, []() { return Scenes::Rooms(); }},
      {Types::Scene::CreateRoom, []() { return Scenes::CreateRoom(); }},
      {Types::Scene::Room, []() { return Scenes::Room(); }},
      {Types::Scene::Login, []() { return Scenes::Login(); }},
      {Types::Scene::Register, []() { return Scenes::Register(); }},
  };

  while (scene != Types::Scene::Exit) {
    auto it = scenes.find(scene);
    if (it != scenes.end()) {
      scene = it->second();
    } else {
      break;
    }
  }
}
