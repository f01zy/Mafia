#pragma once

#include "Types.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace nlohmann {
template <> struct adl_serializer<Types::Room> {
  static void from_json(const json &j, Types::Room &room) {
    room.owner = j.at("owner").get<std::string>();
    room.name = j.at("name").get<std::string>();
    room.maxPlayers = j.at("maxPlayers").get<int>();
    room.players = j.at("players").get<std::vector<std::string>>();
    room.state = j.at("state").get<std::string>();
  }
};

template <> struct adl_serializer<Types::Message> {
  static void from_json(const json &j, Types::Message &message) {
    message.username = j.at("username").get<std::string>();
    message.message = j.at("message").get<std::string>();
  }
};
} // namespace nlohmann
