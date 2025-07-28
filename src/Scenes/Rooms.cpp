#include "../Config/State.h"
#include "../Network/Socket.h"
#include "../Utils/Utils.h"
#include "Scenes.h"
#include "ftxui/component/component.hpp"
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>

Types::Scene Scenes::Rooms() {
  using namespace ftxui;

  State &state = State::getInstance();
  Socket &socket = Socket::getInstance();

  socket.emit("getRooms");
  while (state.isLoading) {
    Utils::sleep(0.5);
  }

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
  auto connectButton =
      Button("Connect", [&] { state.screen.Exit(); }, buttonOption);
  bool isCreateRoomButtonCalled = false;
  auto createRoomButton = Button(
      "Create room",
      [&] {
        isCreateRoomButtonCalled = true;
        state.screen.Exit();
      },
      buttonOption);
  bool isBackButtonCalled = false;
  auto backButton = Button(
      "Back",
      [&] {
        isBackButtonCalled = true;
        state.screen.Exit();
      },
      buttonOption);

  std::vector<std::string> rooms;
  for (Types::Room room : state.rooms) {
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
  state.screen.Loop(renderer);
  state.rooms.clear();

  if (isBackButtonCalled) {
    return Types::Scene::Menu;
  }

  if (isCreateRoomButtonCalled) {
    return Types::Scene::CreateRoom;
  }

  socket.emit("connectToRoom", rooms[selected]);
  while (state.isLoading) {
    Utils::sleep(0.5);
  }

  return Types::Scene::Room;
}
