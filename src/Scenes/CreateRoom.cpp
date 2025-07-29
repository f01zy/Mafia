#include "../Config/Config.h"
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

  Config &config = Config::getInstance();
  State &state = State::getInstance();
  Socket &socket = Socket::getInstance();

  int maxPlayers = config.MIN_ROOM_PLAYERS;
  bool isBackButtonCalled = false;
  std::string title = "Create room";
  std::string name;

  InputOption inputOption;
  inputOption.multiline = false;
  Component nameInput = Input(&name, "name", inputOption);

  Component maxPlayersSlider =
      Slider("Players: ", &maxPlayers, config.MIN_ROOM_PLAYERS,
             config.MAX_ROOM_PLAYERS, 1);

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

  auto createButton =
      Button("Create", [&] { state.screen.Exit(); }, buttonOption);

  auto backButton = Button(
      "Back",
      [&] {
        isBackButtonCalled = true;
        state.screen.Exit();
      },
      buttonOption);

  auto container = Container::Vertical(
      {nameInput, maxPlayersSlider, createButton, backButton});

  auto renderer = Renderer(container, [&] {
    Element content = vbox({
        text(title) | bold,
        separator(),
        hbox(text("Name: "), nameInput->Render() | size(WIDTH, EQUAL, 30)),
        hbox(maxPlayersSlider->Render(), filler() | size(WIDTH, EQUAL, 1),
             text(std::to_string(maxPlayers))),
        separator(),
        createButton->Render(),
        backButton->Render(),
    });

    return center(content | border | size(WIDTH, GREATER_THAN, 40));
  });

  while (1) {
    state.screen.Loop(renderer);

    if (isBackButtonCalled) {
      return Types::Scene::Rooms;
    }

    if (name.size() < 4 || name.size() > 20) {
      title = "Name too short or long";
      continue;
    }

    break;
  }

  json data;
  data["name"] = name;
  data["maxPlayers"] = maxPlayers;

  socket.emit("createRoom", data.dump());
  Utils::loading();

  return Types::Scene::Room;
}
