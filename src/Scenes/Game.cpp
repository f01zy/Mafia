#include "../Config/State.h"
#include "../Network/Socket.h"
#include "Scenes.h"
#include "ftxui/component/component.hpp"
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

Types::Scene Scenes::Game() {
  using namespace ftxui;

  State &state = State::getInstance();
  Socket &socket = Socket::getInstance();

  auto voting = Renderer([] { return text("Voting") | flex | border; });
  auto chat = Renderer([] { return text("Chat") | flex | border; });

  auto container = Container::Horizontal({voting, chat});
  auto renderer = Renderer(container, [&] { return container->Render(); });

  state.screen.Loop(renderer);

  return Types::Scene::Menu;
}
