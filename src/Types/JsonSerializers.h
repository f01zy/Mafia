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
  }
};
} // namespace nlohmann
