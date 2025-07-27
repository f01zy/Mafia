#include "../Config/Config.h"
#include "../Network/Socket.h"
#include "../Utils/Core.h"
#include "Scenes.h"
#include "ftxui/component/component.hpp"
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Types::Scene Scenes::CreateRoom() {
  using namespace ftxui;

  Socket &socket = Socket::getInstance();
  Config &config = Config::getInstance();
  auto screen = ScreenInteractive::Fullscreen();

  std::string name;
  InputOption inputOption;
  inputOption.multiline = false;
  Component nameInput = Input(&name, "name", inputOption);

  int maxPlayers = 10;
  Component maxPlayersSlider = Slider("Players: ", &maxPlayers, 6, 16, 1);

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
  auto createButton = Button("Create", [&] { screen.Exit(); }, buttonOption);

  auto container =
      Container::Vertical({nameInput, maxPlayersSlider, createButton});
  auto component = CatchEvent(container, [&](Event event) {
    if (event == Event::Escape) {
      screen.Exit();
      return 1;
    }

    return 0;
  });

  std::string title = "Create room";

  auto renderer = Renderer(component, [&] {
    return center(vbox({
                      text(title) | bold,
                      separator(),
                      hbox(text("Name   : "),
                           nameInput->Render() | size(WIDTH, EQUAL, 30)),
                      hbox(maxPlayersSlider->Render(),
                           text(" " + std::to_string(maxPlayers))),
                      separator(),
                      createButton->Render(),
                  }) |
                  border);
  });

  while (1) {
    screen.Loop(renderer);

    if (name.size() < 4 || name.size() > 20) {
      title = "Name lenght must be more than 4 and not exceed 20";
    }

    else {
      break;
    }
  }

  json data;
  data["owner"] = config.user["username"];
  data["name"] = name;
  data["maxPlayers"] = maxPlayers;
  socket.emit("createRoom", data.dump());

  while (config.room.name.empty()) {
    Utils::Core::sleep(0.5);
  }

  return Types::Scene::Room;
}
