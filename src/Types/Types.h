#pragma once

#include <string>
#include <vector>

namespace Types {
enum class Scene { Menu, Rooms, CreateRoom, Room, Game, Login, Register, Exit };

struct Message {
  std::string username;
  std::string message;
};

struct Room {
  std::string owner;
  std::string name;
  int maxPlayers;
  std::vector<std::string> players;
  std::vector<Message> messages;
  std::string state;
};

struct User {
  int id;
  std::string username;
  std::string email;
  bool isActivated;
};
} // namespace Types
