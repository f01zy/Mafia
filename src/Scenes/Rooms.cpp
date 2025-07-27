#include "../Config/Config.h"
#include "../Network/Socket.h"
#include "../Utils/Core.h"
#include "Scenes.h"
#include "ftxui/component/component.hpp"
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>

Types::Scene Scenes::Rooms() {
  using namespace ftxui;

  Config &config = Config::getInstance();
  Socket &socket = Socket::getInstance();

  socket.emit("getRooms");
  while (config.getIsLoading()) {
    Utils::Core::sleep(0.5);
  }

  auto screen = ScreenInteractive::Fullscreen();

  auto buttonOption = ButtonOption::Simple();
  buttonOption.transform = [](const EntryState &s) {
    Element element = text(s.label);
    if (s.focused) {
      element = element | bold | bgcolor(Color::Blue) | color(Color::White);
    }

    else {
      element = element | color(Color::GrayDark);
    }
    return element | borderEmpty;
  };
  auto connectButton = Button("Connect", [&] { screen.Exit(); }, buttonOption);
  bool isCreateRoomButtonCalled = false;
  auto createRoomButton = Button(
      "Create room",
      [&] {
        isCreateRoomButtonCalled = true;
        screen.Exit();
      },
      buttonOption);
  bool isBackButtonCalled = false;
  auto backButton = Button(
      "Back",
      [&] {
        isBackButtonCalled = true;
        screen.Exit();
      },
      buttonOption);

  std::vector<std::string> rooms;
  for (Types::Room room : config.rooms) {
    rooms.push_back(room.name);
  }

  int selected = 0;
  Component dropdown = Dropdown(rooms, &selected);

  auto component = Container::Vertical(
      {dropdown, connectButton, createRoomButton, backButton});

  auto renderer = Renderer(component, [&] {
    return center(vbox({text("Rooms") | bold, separator(), dropdown->Render(),
                        separator(), connectButton->Render(),
                        createRoomButton->Render(), backButton->Render()}) |
                  border | size(WIDTH, GREATER_THAN, 30));
  });
  screen.Loop(renderer);
  config.rooms.clear();

  if (isBackButtonCalled) {
    return Types::Scene::Menu;
  }

  if (isCreateRoomButtonCalled) {
    return Types::Scene::CreateRoom;
  }

  socket.emit("connectToRoom", rooms[selected]);
  while (config.getIsLoading()) {
    Utils::Core::sleep(0.5);
  }

  return Types::Scene::Room;
}
