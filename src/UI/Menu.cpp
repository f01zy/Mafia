#include "Menu.h"
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include <ftxui/dom/elements.hpp>

int UI::Menu::draw() {
  using namespace ftxui;

  auto screen = ScreenInteractive::Fullscreen();
  int selected = 0;

  auto menu = ftxui::Menu(&options, &selected);

  auto component = CatchEvent(menu, [&](Event event) {
    if (event == Event::Return) {
      screen.Exit();
      return 1;
    }

    return 0;
  });

  auto renderer = Renderer(component, [&] {
    return center(vbox(text(title) | bold, separator(), component->Render()) |
                  border | size(WIDTH, GREATER_THAN, 30));
  });
  screen.Loop(renderer);

  return selected;
}
