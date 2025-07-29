#include "../Config/Config.h"
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
  Config &config = Config::getInstance();
  Socket &socket = Socket::getInstance();

  Types::User &user = state.getUser();
  Types::Room &room = state.getRoom();

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

  while (1) {
    state.isRoomUpdated = false;

    bool isOwner = user.username == room.owner;
    bool hasMinimumPlayers = room.players.size() >= config.MIN_ROOM_PLAYERS;

    bool isShowStartButton = isOwner && hasMinimumPlayers;
    bool isStartButtonCalled = false;
    auto startButton = Maybe(Button(
                                 "Start",
                                 [&] {
                                   isStartButtonCalled = true;
                                   socket.emit("startGame");
                                   state.screen.Exit();
                                 },
                                 buttonOption),
                             &isShowStartButton);

    bool isDisconnectButtonCalled = false;
    auto disconnectButton = Button(
        "Disconnect",
        [&] {
          isDisconnectButtonCalled = true;
          socket.emit("disconnectFromRoom");
          state.screen.Exit();
        },
        buttonOption);

    Component container = Container::Vertical({startButton, disconnectButton});

    auto renderer = Renderer(container, [&] {
      std::string playersLabel = std::to_string(room.players.size()) + "/" +
                                 std::to_string(room.maxPlayers) + " players";

      Element content = vbox({
          text(room.name) | bold,
          text(playersLabel),
          separator(),
          startButton->Render(),
          disconnectButton->Render(),
      });

      return center(content | border | size(WIDTH, GREATER_THAN, 40));
    });
    state.screen.Loop(renderer);

    if (isDisconnectButtonCalled) {
      return Types::Scene::Menu;
    }

    if (isStartButtonCalled) {
      return Types::Scene::Game;
    }

    if (!state.isRoomUpdated) {
      break;
    }
  }
}
