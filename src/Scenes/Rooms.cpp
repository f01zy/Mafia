#include "Scenes.h"
#include "ftxui/component/component.hpp"
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>
#include <vector>

std::vector<std::string> rooms = {
    "tribute",     "clearance", "ally",        "bend",        "electronics",
    "module",      "era",       "cultural",    "sniff",       "nationalism",
    "negotiation", "deliver",   "figure",      "east",        "tribute",
    "clearance",   "ally",      "bend",        "electronics", "module",
    "era",         "cultural",  "sniff",       "nationalism", "negotiation",
    "deliver",     "figure",    "east",        "tribute",     "clearance",
    "ally",        "bend",      "electronics", "module",      "era",
    "cultural",    "sniff",     "nationalism", "negotiation", "deliver",
    "figure",      "east",
};

Types::Scene Scenes::Rooms() {
  using namespace ftxui;

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
  auto connectButton = Button("Connect", [&] { screen.Exit(); }, buttonOption);
  bool isCreateRoomButtonCalled = false;
  auto createRoomButton = Button(
      "Create room",
      [&] {
        isCreateRoomButtonCalled = true;
        screen.Exit();
      },
      buttonOption);

  int selected = 0;
  Component dropdown = Dropdown(&rooms, &selected);

  auto container =
      Container::Vertical({dropdown, connectButton, createRoomButton});
  auto component = CatchEvent(container, [&](Event event) {
    if (event == Event::Escape) {
      screen.Exit();
      return 1;
    }

    return 0;
  });

  auto renderer = Renderer(component, [&] {
    return center(vbox({
                      text("Rooms") | bold,
                      separator(),
                      dropdown->Render(),
                      separator(),
                      connectButton->Render(),
                      createRoomButton->Render(),
                  }) |
                  border | size(WIDTH, GREATER_THAN, 30));
  });
  screen.Loop(renderer);

  if (isCreateRoomButtonCalled) {
    return Types::Scene::CreateRoom;
  }

  return Types::Scene::Menu;
}
