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

  bool isConnectButtonCalled = false;
  auto connectButton = Button(
      "Connect",
      [&] {
        isConnectButtonCalled = true;
        state.screen.Exit();
      },
      buttonOption);

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

  bool isEmpty = rooms.size() == 0;
  auto component = isEmpty
                       ? Container::Vertical({createRoomButton, backButton})
                       : Container::Vertical({dropdown, connectButton,
                                              createRoomButton, backButton});

  auto buttons = [&] {
    return isEmpty ? vbox({createRoomButton->Render(), backButton->Render()})
                   : vbox({connectButton->Render(), createRoomButton->Render(),
                           backButton->Render()});
  };

  std::string title = "Rooms";

  auto renderer = Renderer(component, [&] {
    return center(
        vbox({
            text(title) | bold,
            separator(),
            isEmpty ? text("There are no rooms yet") : dropdown->Render(),
            separator(),
            buttons(),
        }) |
        border | size(WIDTH, GREATER_THAN, 30));
  });

  while (1) {
    state.screen.Loop(renderer);

    if (isBackButtonCalled) {
      state.rooms.clear();
      return Types::Scene::Menu;
    }

    if (isCreateRoomButtonCalled) {
      state.rooms.clear();
      return Types::Scene::CreateRoom;
    }

    if (isConnectButtonCalled) {
      socket.emit("connectToRoom", rooms[selected]);
      while (state.isLoading) {
        Utils::sleep(0.5);
      }

      if (!state.error.empty()) {
        title = state.error;
        state.error.clear();
        continue;
      }
    }

    state.rooms.clear();
    break;
  }

  return Types::Scene::Room;
}
