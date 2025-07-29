#include "../Config/State.h"
#include "../Network/Socket.h"
#include "../Utils/Auth.h"
#include "Scenes.h"
#include "ftxui/component/component.hpp"
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

Types::Scene Scenes::Login() {
  using namespace ftxui;

  State &state = State::getInstance();
  Socket &socket = Socket::getInstance();

  bool isRegisterButtonCalled = false;
  std::string title = "Login";
  std::string username;
  std::string password;

  InputOption inputOption;
  inputOption.multiline = false;
  Component usernameInput = Input(&username, "Username", inputOption);
  inputOption.password = true;
  Component passwordInput = Input(&password, "Password", inputOption);

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

  Component loginButton =
      Button("Login", [&] { state.screen.Exit(); }, buttonOption);

  Component registerButton = Button(
      "Don't have a accont? - Register",
      [&] {
        isRegisterButtonCalled = true;
        state.screen.Exit();
      },
      buttonOption);

  Component container = Container::Vertical(
      {usernameInput, passwordInput, loginButton, registerButton});

  auto renderer = Renderer(container, [&] {
    Element content = vbox({
        text(title) | bold,
        separator(),
        hbox(text("Email: "), usernameInput->Render()),
        hbox(text("Password: "), passwordInput->Render()),
        separator(),
        loginButton->Render(),
        registerButton->Render(),
    });

    return center(content | border | size(WIDTH, GREATER_THAN, 40));
  });

  while (1) {
    state.screen.Loop(renderer);

    if (isRegisterButtonCalled) {
      return Types::Scene::Register;
    }

    std::string message = Utils::Auth::signIn(username, password);
    if (!message.empty()) {
      title = message;
    }

    else {
      socket.auth();
      break;
    }
  }

  return Types::Scene::Menu;
}
