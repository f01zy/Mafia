#pragma once

#include "../Types/Types.h"
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

class Config final {
public:
  static Config &getInstance() {
    static Config config;
    return config;
  };

  std::vector<Types::Room> rooms;
  const unsigned WIDTH = 600, HEIGHT = 600;
  const std::string API_URL = "http://localhost:3000";

  Types::User &getUser() { return user; };
  void setUser(Types::User &newUser) { user = newUser; };

  Types::Room &getRoom() { return room; };
  void setRoom(Types::Room &newRoom) { room = newRoom; };

  bool getIsLoading() { return isLoading; };
  void setIsLoading(bool state) { isLoading = state; };

private:
  Types::User user;
  Types::Room room;
  bool isLoading = 0;
};
