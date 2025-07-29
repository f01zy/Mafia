#include "../Config/State.h"
#include "../Network/Socket.h"
#include "../Utils/Utils.h"
#include "Scenes.h"
#include "ftxui/component/component.hpp"
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Types::Scene Scenes::CreateRoom() {
  using namespace ftxui;

  State &state = State::getInstance();
  Socket &socket = Socket::getInstance();

  std::string name;
  InputOption inputOption;
  inputOption.multiline = false;
  Component nameInput = Input(&name, "name", inputOption);

  int maxPlayers = 8;
  Component maxPlayersSlider = Slider("Players: ", &maxPlayers, 1, 12, 1);

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

  auto createButton =
      Button("Create", [&] { state.screen.Exit(); }, buttonOption);

  bool isBackButtonCalled = false;
  auto backButton = Button(
      "Back",
      [&] {
        isBackButtonCalled = true;
        state.screen.Exit();
      },
      buttonOption);

  auto component = Container::Vertical(
      {nameInput, maxPlayersSlider, createButton, backButton});

  std::string title = "Create room";

  auto renderer = Renderer(component, [&] {
    return center(
        vbox({
            text(title) | bold,
            separator(),
            hbox(text("Name   : "),
                 nameInput->Render() | size(WIDTH, EQUAL, 30)),
            hbox(maxPlayersSlider->Render(), filler() | size(WIDTH, EQUAL, 1),
                 text(std::to_string(maxPlayers))),
            separator(),
            createButton->Render(),
            backButton->Render(),
        }) |
        border);
  });

  while (1) {
    state.screen.Loop(renderer);

    if (isBackButtonCalled) {
      return Types::Scene::Rooms;
    }

    if (name.size() < 4 || name.size() > 20) {
      title = "Name lenght must be more than 4 and not exceed 20";
      continue;
    }

    break;
  }

  json data;
  data["name"] = name;
  data["maxPlayers"] = maxPlayers;

  socket.emit("createRoom", data.dump());
  while (state.isLoading) {
    Utils::sleep(0.5);
  }

  return Types::Scene::Room;
}
