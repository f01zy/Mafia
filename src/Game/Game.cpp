#include "Game.h"
#include "../Config/State.h"
#include "../Network/Socket.h"
#include "../Scenes/Scenes.h"
#include "../Utils/Auth.h"
#include <unordered_map>

void Game::run() {
  State &state = State::getInstance();
  Socket &socket = Socket::getInstance();

  bool isAuth = Utils::Auth::checkAuth();
  state.scene = isAuth ? Types::Scene::Game : Types::Scene::Login;

  if (isAuth) {
    socket.auth();
  }

  std::unordered_map<Types::Scene, std::function<Types::Scene()>> scenes = {
      {Types::Scene::Menu, []() { return Scenes::Menu(); }},
      {Types::Scene::Rooms, []() { return Scenes::Rooms(); }},
      {Types::Scene::CreateRoom, []() { return Scenes::CreateRoom(); }},
      {Types::Scene::Room, []() { return Scenes::Room(); }},
      {Types::Scene::Game, []() { return Scenes::Game(); }},
      {Types::Scene::Login, []() { return Scenes::Login(); }},
      {Types::Scene::Register, []() { return Scenes::Register(); }},
  };

  while (state.scene != Types::Scene::Exit) {
    auto it = scenes.find(state.scene);
    if (it != scenes.end()) {
      state.scene = it->second();
    } else {
      break;
    }
  }
}
