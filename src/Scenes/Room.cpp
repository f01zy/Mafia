#include "../Config/State.h"
#include "../Network/Socket.h"
#include "Scenes.h"
#include "ftxui/component/component.hpp"
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

Types::Scene Scenes::Room() {
  using namespace ftxui;

  State &state = State::getInstance();
  Socket &socket = Socket::getInstance();

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
  auto startButton =
      Button("Start", [&] { state.screen.Exit(); }, buttonOption);
  auto disconnectButton = Button(
      "Disconnect",
      [&] {
        socket.emit("disconnectFromRoom");
        state.screen.Exit();
      },
      buttonOption);

  Types::User &user = state.getUser();
  Types::Room &room = state.getRoom();

  while (1) {
    state.isRoomUpdated = false;
    bool isOwner = user.username == room.owner;
    Component component =
        isOwner ? Container::Vertical({startButton, disconnectButton})
                : Container::Vertical({disconnectButton});

    auto buttons = [&] {
      return isOwner ? vbox({startButton->Render(), disconnectButton->Render()})
                     : disconnectButton->Render();
    };

    auto renderer = Renderer(component, [&] {
      return center(vbox({
                        text(room.name) | bold,
                        text(std::to_string(room.players.size()) + "/" +
                             std::to_string(room.maxPlayers) + " players"),
                        separator(),
                        buttons(),
                    }) |
                    border | size(WIDTH, GREATER_THAN, 30));
    });
    state.screen.Loop(renderer);

    if (!state.isRoomUpdated) {
      break;
    }
  }

  return Types::Scene::Menu;
}
