#include "../Config/State.h"
#include "../Network/Socket.h"
#include "../Utils/Utils.h"
#include "Scenes.h"
#include "ftxui/component/component.hpp"
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/deprecated.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>

Types::Scene Scenes::Rooms() {
  using namespace ftxui;

  State &state = State::getInstance();
  Socket &socket = Socket::getInstance();

  socket.emit("getRooms");
  Utils::loading();

  std::string title = "Rooms";
  bool isConnectButtonCalled = false;
  bool isCreateRoomButtonCalled = false;
  bool isBackButtonCalled = false;
  int selectedRoom = 0;

  std::vector<std::string> rooms;
  for (Types::Room room : state.rooms) {
    rooms.push_back(room.name);
  }
  bool hasRooms = rooms.size() > 0;

  ButtonOption buttonOption = ButtonOption::Simple();
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

  Component connectButton = Button(
      "Connect",
      [&] {
        isConnectButtonCalled = true;
        state.screen.Exit();
      },
      buttonOption);

  Component createRoomButton = Button(
      "Create room",
      [&] {
        isCreateRoomButtonCalled = true;
        state.screen.Exit();
      },
      buttonOption);

  Component backButton = Button(
      "Back",
      [&] {
        isBackButtonCalled = true;
        state.screen.Exit();
      },
      buttonOption);

  Component roomsDropdown = Dropdown(rooms, &selectedRoom);

  Component container =
      !hasRooms ? Container::Vertical({createRoomButton, backButton})
                : Container::Vertical({roomsDropdown, connectButton,
                                       createRoomButton, backButton});

  Component renderer = Renderer(container, [&] {
    Element content;

    if (hasRooms) {
      content = vbox({
          text(title) | bold,
          separator(),
          roomsDropdown->Render(),
          separator(),
          connectButton->Render(),
          createRoomButton->Render(),
          backButton->Render(),
      });
    }

    else {
      content = vbox({
          text("No rooms available") | bold,
          separator(),
          createRoomButton->Render(),
          backButton->Render(),
      });
    }

    return center(content | border | size(WIDTH, GREATER_THAN, 40));
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
      socket.emit("connectToRoom", rooms[selectedRoom]);
      Utils::loading();

      if (!state.error.empty()) {
        title = state.error;
        state.error.clear();
        continue;
      }

      state.rooms.clear();
      return Types::Scene::Room;
    }

    break;
  }
}
