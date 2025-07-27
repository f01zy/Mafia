#include "Socket.h"
#include "../Config/Config.h"
#include "../Utils/Core.h"

void receiveCreateRoom(sio::event event) {
  Config &config = Config::getInstance();
  json data = json::parse(event.get_message()->get_string());
  config.room.owner = data["owner"];
  config.room.name = data["name"];
  config.room.maxPlayers = data["maxPlayers"];
  config.room.players = data["players"].get<std::vector<std::string>>();
}

void receiveRooms(sio::event event) {
  Config &config = Config::getInstance();
  json data = json::parse(event.get_message()->get_string());
  std::vector<std::string> rooms =
      data["rooms"].get<std::vector<std::string>>();
  std::vector<std::string> noRooms = {"There are no rooms yet"};
  config.rooms = rooms.empty() ? noRooms : rooms;
}

Socket::Socket(std::string url) {
  on("receiveCreateRoom", receiveCreateRoom);
  on("receiveRooms", receiveRooms);
  c.set_logs_quiet();
  c.connect(url);
}

Socket::~Socket() {
  this->c.clear_con_listeners();
  this->c.sync_close();
}

void Socket::emit(std::string event, std::string data) {
  c.socket()->emit(event, data);
}

void Socket::on(std::string event, std::function<void(sio::event &)> callback) {
  c.socket()->on(event, callback);
}

Socket &Socket::getInstance() {
  Config config = Config::getInstance();
  static Socket instance(config.API_URL);
  return instance;
}
