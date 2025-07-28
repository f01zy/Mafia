#pragma once

#include "../Types/Types.h"
#include <ftxui/component/screen_interactive.hpp>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

class State final {
public:
  static State &getInstance() {
    static State config;
    return config;
  };

  ftxui::ScreenInteractive screen = ftxui::ScreenInteractive::Fullscreen();
  std::vector<Types::Room> rooms;

  Types::User &getUser() { return user; };
  Types::Room &getRoom() { return room; };
  std::string &getError() { return error; };
  bool getIsLoading() { return isLoading; };
  void setUser(Types::User &newUser) { user = newUser; };
  void setRoom(Types::Room &newRoom) { room = newRoom; };
  void setError(std::string newError) { error = newError; };
  void setIsLoading(bool state) { isLoading = state; };

private:
  Types::User user;
  Types::Room room;
  std::string error;
  bool isLoading = 0;
};
