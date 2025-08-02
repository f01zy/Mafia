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

  Component container =
      Container::Vertical({messageInput, sendMessageButton, disconnectButton});
  Component renderer = Renderer(container, [&] {
    std::vector<Element> messages;

    for (const Types::Message &message : room.messages) {
      messages.push_back(text(message.username + ": " + message.content));
    }

    std::string title = room.name + ", " + std::to_string(room.players.size()) +
                        "/" + std::to_string(room.maxPlayers) + " players";

    Element content = vbox({
        text(title),
        separator(),
        vbox(messages) | yframe | flex,
        separator(),
        messageInput->Render(),
        separator(),
        sendMessageButton->Render(),
        disconnectButton->Render(),
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
