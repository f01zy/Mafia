#include "Json.h"

Types::Room Utils::Json::jsonToRoom(json data) {
  Types::Room room;
  room.owner = data["owner"];
  room.name = data["name"];
  room.maxPlayers = data["maxPlayers"];
  room.players = data["players"].get<std::vector<std::string>>();
  return room;
}

Types::User Utils::Json::jsonToUser(json data) {
  Types::User user;
  user.id = data["id"];
  user.username = data["username"];
  user.email = data["email"];
  user.isActivated = data["isActivated"];
  return user;
}
