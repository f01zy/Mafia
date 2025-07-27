#pragma once

#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

struct Room {
  std::string owner;
  std::string name;
  int maxPlayers;
  std::vector<std::string> players;
};

class Config final {
public:
  static Config &getInstance() {
    static Config config;
    return config;
  };

  json user = json::object();
  Room room;
  std::vector<std::string> rooms{};

  const unsigned WIDTH = 600, HEIGHT = 600;
  const std::string API_URL = "http://localhost:3000";
};
