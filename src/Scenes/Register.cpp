#include "../Utils/Auth.h"
#include "Scenes.h"
#include "ftxui/component/component.hpp"
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

Types::Scene Scenes::Register(std::string title) {
  using namespace ftxui;

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
  bool isLoginButtonCalled = false;
  auto loginButton = Button(
      "Already registered? - login", [&] { isLoginButtonCalled = true; },
      buttonOption);

  auto renderInput = [](Component input) {
    return input->Render() | size(WIDTH, EQUAL, 30);
  };

  auto renderButton = [](Component button) { return button->Render(); };

  auto container =
      Container::Vertical({usernameInput, emailInput, passwordInput,
                           confirmPasswordInput, loginButton});
  auto component = CatchEvent(container, [&](Event event) {
    if (event == Event::Return || isLoginButtonCalled) {
      screen.Exit();
      return 1;
    }

    return 0;
  });

  while (1) {
    auto renderer = Renderer(component, [&] {
      return center(vcenter(
          vbox({
              text(title) | bold,
              filler() | size(HEIGHT, EQUAL, 1),
              hbox(text("Username         : "), renderInput(usernameInput)),
              hbox(text("Email            : "), renderInput(emailInput)),
              hbox(text("Password         : "), renderInput(passwordInput)),
              hbox(text("Confirm password : "),
                   renderInput(confirmPasswordInput)),
              renderButton(loginButton),
          }) |
          border));
    });
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
      break;
    }
  }

  return Types::Scene::Lobby;
}
