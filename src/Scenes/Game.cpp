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

  std::string message;
  const Types::Room &room = state.getRoom();
  const Types::User &user = state.getUser();
  int tabSelected = 0;

  std::vector<std::string> tabValues = {"Chat", "Players", "Voting"};
  Component tabToggle = ftxui::Toggle(&tabValues, &tabSelected);

  InputOption inputOption;
  inputOption.multiline = false;
  Component messageInput = Input(&message, "Type your message...", inputOption);

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

  bool isSendMessageButtonCalled = false;
  auto sendMessageButton = Button(
      "Send",
      [&] {
        isSendMessageButtonCalled = true;
        state.screen.Exit();
      },
      buttonOption);

  bool isDisconnectButtonCalled = false;
  auto disconnectButton = Button(
      "Disconnect",
      [&] {
        isDisconnectButtonCalled = true;
        socket.emit("disconnectFromRoom");
        state.screen.Exit();
      },
      buttonOption);

  Component chatContainer =
      Container::Vertical({messageInput, sendMessageButton, disconnectButton});
  Component chatRenderer = Renderer(chatContainer, [&] {
    std::vector<Element> messages;

    for (const Types::Message &message : room.messages) {
      messages.push_back(text(message.username + ": " + message.content));
    }

    Element content = vbox({
        text(room.name),
        separator(),
        vbox(messages) | yframe | flex,
        separator(),
        messageInput->Render(),
        separator(),
        sendMessageButton->Render(),
        disconnectButton->Render(),
    });

    return content;
  });

  Component playersContainer = Container::Vertical({});
  Component playersRenderer = Renderer(playersContainer, [&] {
    std::vector<Element> players;

    for (const Types::Player &player : room.players) {
      std::string label = player.username;

      if (player.username == user.username) {
        label = player.username + ": " + player.role;
      }

      players.push_back(text(label));
    }

    Element content = vbox({
        text("Players"),
        separator(),
        vbox(players) | yframe | flex,
    });

    return content;
  });

  Component votingContainer = Container::Vertical({});
  Component votingRenderer = Renderer(votingContainer, [&] {
    Element content = vbox({
        text("Voting"),
    });

    return content;
  });

  Component tabContainer = Container::Tab(
      {chatRenderer, playersRenderer, votingRenderer}, &tabSelected);
  Component container = Container::Vertical({tabToggle, tabContainer});
  Component renderer = Renderer(container, [&] {
    Element content = vbox({
        tabToggle->Render(),
        separator(),
        tabContainer->Render() | flex,
    });

    return content | flex | border;
  });

  while (1) {
    state.screen.Loop(renderer);

    if (isSendMessageButtonCalled) {
      if (message.size() > 0) {
        socket.emit("sendMessage", message);
        message = "";
      }

      isSendMessageButtonCalled = false;
    }

    if (isDisconnectButtonCalled) {
      break;
    }
  }

  return Types::Scene::Menu;
}
