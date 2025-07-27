#include "../Config/Config.h"
#include "Scenes.h"
#include "ftxui/component/component.hpp"
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>

Types::Scene Scenes::Room() {
  using namespace ftxui;

  Config &config = Config::getInstance();
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
  auto disconnectButton = Button(
      "Disconnect",
      [&] {
        config.room.name.clear();
        config.room.maxPlayers = 0;
        config.room.owner.clear();
        config.room.players.clear();
        screen.Exit();
      },
      buttonOption);

  auto component = Container::Vertical({disconnectButton});
  auto renderer = Renderer(component, [&] {
    return center(vbox({
                      text(config.room.name) | bold,
                      text(std::to_string(config.room.players.size()) + "/" +
                           std::to_string(config.room.maxPlayers) + " players"),
                      separator(),
                      disconnectButton->Render(),
                  }) |
                  border | size(WIDTH, GREATER_THAN, 30));
  });
  screen.Loop(renderer);

  return Types::Scene::Menu;
}
