#include "../UI/Menu.h"
#include "../Utils/Auth.h"
#include "Scenes.h"
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

Types::Scene Scenes::Menu() {
  std::vector<std::string> options = {"Rooms", "Logout", "Exit"};
  UI::Menu menu(options, "Lobby");
  int choice = menu.draw();

  switch (choice) {
  case 0:
    return Types::Scene::Rooms;

  case 1:
    Utils::Auth::logout();
    return Types::Scene::Login;
  }

  return Types::Scene::Exit;
}
