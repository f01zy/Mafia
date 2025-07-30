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
      return true;
    }

    return false;
  });

  auto renderer = Renderer(component, [&] {
    Element content = vbox({
        text(title) | bold,
        separator(),
        component->Render(),
    });

    return center(content | border | size(WIDTH, GREATER_THAN, 40));
  });
  screen.Loop(renderer);

  return selected;
}
