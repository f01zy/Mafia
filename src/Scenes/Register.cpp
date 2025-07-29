#include "../Config/State.h"
#include "../Network/Socket.h"
#include "../Utils/Auth.h"
#include "Scenes.h"
#include "ftxui/component/component.hpp"
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

Types::Scene Scenes::Register() {
  using namespace ftxui;

  State &state = State::getInstance();
  Socket &socket = Socket::getInstance();

  bool isLoginButtonCalled = false;
  std::string title = "Register";
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

  Component registerButton =
      Button("Register", [&] { state.screen.Exit(); }, buttonOption);

  Component loginButton = Button(
      "Already registered? - login",
      [&] {
        isLoginButtonCalled = true;
        state.screen.Exit();
      },
      buttonOption);

  Component container =
      Container::Vertical({usernameInput, emailInput, passwordInput,
                           confirmPasswordInput, registerButton, loginButton});

  auto renderer = Renderer(container, [&] {
    Element content = vbox({
        text(title) | bold,
        separator(),
        hbox(text("Username: "), usernameInput->Render()),
        hbox(text("Email: "), emailInput->Render()),
        hbox(text("Password: "), passwordInput->Render()),
        hbox(text("Confirm password: "), confirmPasswordInput->Render()),
        separator(),
        registerButton->Render(),
        loginButton->Render(),
    });

    return center(content | border | size(WIDTH, GREATER_THAN, 40));
  });

  while (1) {
    state.screen.Loop(renderer);

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
