#pragma once

#include "../Types/Types.h"
#include <ftxui/component/screen_interactive.hpp>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

class State final {
public:
  static State &getInstance() {
    static State state;
    return state;
  };

  std::string error;
  bool isLoading = false;
  bool isRoomUpdated = false;
  ftxui::ScreenInteractive screen = ftxui::ScreenInteractive::Fullscreen();
  std::vector<Types::Room> rooms;
  Types::Scene scene;

  const Types::User &getUser() { return user; };
  const Types::Room &getRoom() { return room; };
  void setUser(Types::User &newUser) { user = newUser; };
  void setRoom(Types::Room &newRoom) { room = newRoom; };

private:
  Types::User user;
  Types::Room room;
};
