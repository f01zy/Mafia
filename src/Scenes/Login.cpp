#include "../Utils/Auth.h"
#include "Scenes.h"
#include "ftxui/component/component.hpp"
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

Types::Scene Scenes::Login(std::string title) {
  using namespace ftxui;

  auto screen = ScreenInteractive::Fullscreen();

  std::string username;
  std::string password;

  InputOption inputOption;
  inputOption.multiline = false;
  Component usernameInput = Input(&username, "Username", inputOption);
  inputOption.password = true;
  Component passwordInput = Input(&password, "Password", inputOption);

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
  auto loginButton = Button("Login", [&] { screen.Exit(); }, buttonOption);
  bool isRegisterButtonCalled = false;
  auto registerButton = Button(
      "Don't have a accont? - Register",
      [&] {
        isRegisterButtonCalled = true;
        screen.Exit();
      },
      buttonOption);

  auto container = Container::Vertical(
      {usernameInput, passwordInput, loginButton, registerButton});
  auto component = CatchEvent(container, [&](Event event) {
    if (event == Event::Escape) {
      screen.Exit();
      return 1;
    }

    return 0;
  });

  while (1) {
    auto renderer = Renderer(component, [&] {
      return center(vbox({
                        text(title) | bold,
                        separator(),
                        hbox(text("Email   : "),
                             usernameInput->Render() | size(WIDTH, EQUAL, 30)),
                        hbox(text("Password: "),
                             passwordInput->Render() | size(WIDTH, EQUAL, 30)),
                        separator(),
                        loginButton->Render(),
                        registerButton->Render(),
                    }) |
                    border);
    });
    screen.Loop(renderer);

    if (isRegisterButtonCalled) {
      return Types::Scene::Register;
    }

    std::string message = Utils::Auth::signIn(username, password);
    if (!message.empty()) {
      title = message;
    }

    else {
      break;
    }
  }

  return Types::Scene::Menu;
}
