#include "../Network/Socket.h"
#include "../Utils/Auth.h"
#include "Scenes.h"
#include "ftxui/component/component.hpp"
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

Types::Scene Scenes::Register(std::string title) {
  using namespace ftxui;

  Socket &socket = Socket::getInstance();
  auto screen = ScreenInteractive::Fullscreen();

  std::string username;
  std::string email;
  std::string password;
  std::string confirmPassword;

  InputOption inputOption;
  inputOption.multiline = false;
  Component usernameInput = Input(&username, "Username", inputOption);
  Component emailInput = Input(&email, "Email", inputOption);
  inputOption.password = true;
  Component passwordInput = Input(&password, "Password", inputOption);
  Component confirmPasswordInput =
      Input(&confirmPassword, "Confirm password", inputOption);

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
  auto registerButton =
      Button("Register", [&] { screen.Exit(); }, buttonOption);
  bool isLoginButtonCalled = false;
  auto loginButton = Button(
      "Already registered? - login",
      [&] {
        isLoginButtonCalled = true;
        screen.Exit();
      },
      buttonOption);

  auto component =
      Container::Vertical({usernameInput, emailInput, passwordInput,
                           confirmPasswordInput, registerButton, loginButton});

  auto renderer = Renderer(component, [&] {
    return center(
        vbox({
            text(title) | bold,
            separator(),
            hbox(text("Username        : "),
                 usernameInput->Render() | size(WIDTH, EQUAL, 30)),
            hbox(text("Email           : "),
                 emailInput->Render() | size(WIDTH, EQUAL, 30)),
            hbox(text("Password        : "),
                 passwordInput->Render() | size(WIDTH, EQUAL, 30)),
            hbox(text("Confirm password: "),
                 confirmPasswordInput->Render() | size(WIDTH, EQUAL, 30)),
            separator(),
            registerButton->Render(),
            loginButton->Render(),
        }) |
        border);
  });

  while (1) {
    screen.Loop(renderer);

    if (password != confirmPassword) {
      title = "Passwords don't match";
      continue;
    }

    if (isLoginButtonCalled) {
      return Types::Scene::Login;
    }

    std::string message = Utils::Auth::signUp(username, email, password);
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
