#include "../UI/Menu.h"
#include "../Utils/Auth.h"
#include "Scenes.h"
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

Types::Scene Scenes::Lobby() {
  bool isAuth = Utils::Auth::checkAuth();

  std::vector<std::string> options = {"Rooms", "Exit"};
  UI::Menu menu(options, "Lobby");
  int choice = menu.draw();

  switch (choice) {
  case 0:
    return Types::Scene::Lobby;

  case 1:
    return Types::Scene::Exit;
  }

  return Types::Scene::Exit;
}
