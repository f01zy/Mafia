#include "Scenes.h"
#include "ftxui/component/component.hpp"
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

Types::Scene Scenes::CreateRoom() {
  using namespace ftxui;

  auto screen = ScreenInteractive::Fullscreen();

  std::string name;
  InputOption inputOption;
  inputOption.multiline = false;
  Component nameInput = Input(&name, "name", inputOption);

  int players = 10;
  Component playersSlider = Slider("Players: ", &players, 6, 16, 1);

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
      Container::Vertical({nameInput, playersSlider, createButton});
  auto component = CatchEvent(container, [&](Event event) {
    if (event == Event::Escape) {
      screen.Exit();
      return 1;
    }

    return 0;
  });

  auto renderer = Renderer(component, [&] {
    return center(
        vbox({
            text("Create room") | bold,
            separator(),
            hbox(text("Name   : "),
                 nameInput->Render() | size(WIDTH, EQUAL, 30)),
            hbox(playersSlider->Render(), text(" " + std::to_string(players))),
            separator(),
            createButton->Render(),
        }) |
        border);
  });
  screen.Loop(renderer);

  return Types::Scene::Menu;
}
