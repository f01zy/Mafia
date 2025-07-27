#include "../Config/Config.h"
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

  Config &config = Config::getInstance();
  Socket &socket = Socket::getInstance();
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
  auto startButton = Button("Start", [&] { screen.Exit(); }, buttonOption);
  auto disconnectButton = Button(
      "Disconnect",
      [&] {
        socket.emit("disconnectFromRoom");
        screen.Exit();
      },
      buttonOption);

  Types::User &user = config.getUser();
  Types::Room &room = config.getRoom();

  auto component = Container::Vertical({disconnectButton});
  auto renderer = Renderer(component, [&] {
    return center(vbox({
                      text(room.name) | bold,
                      text(std::to_string(room.players.size()) + "/" +
                           std::to_string(room.maxPlayers) + " players"),
                      separator(),
                      disconnectButton->Render(),
                  }) |
                  border | size(WIDTH, GREATER_THAN, 30));
  });
  screen.Loop(renderer);

  return Types::Scene::Menu;
}
