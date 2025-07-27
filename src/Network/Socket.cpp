#include "Socket.h"
#include "../Config/Config.h"
#include "../Types/JsonSerializers.h"
#include "../Utils/Json.h"

void updateRoom(sio::event event) {
  Config &config = Config::getInstance();
  json data = json::parse(event.get_message()->get_string());
  Types::Room room = Utils::Json::jsonToRoom(data);
  config.setRoom(room);
  config.setIsLoading(0);
}

void receiveRooms(sio::event event) {
  Config &config = Config::getInstance();
  json data = json::parse(event.get_message()->get_string());
  config.rooms = data["rooms"].get<std::vector<Types::Room>>();
  config.setIsLoading(0);
}

void receiveDisconnectFromRoom(sio::event event) {
  Config &config = Config::getInstance();
  Types::Room room;
  config.setRoom(room);
  config.setIsLoading(0);
}

Socket::Socket(std::string url) {
  on("receiveDisconnectFromRoom", receiveDisconnectFromRoom);
  on("updateRoom", updateRoom);
  on("receiveRooms", receiveRooms);

  c.set_logs_quiet();
  c.connect(url);
}

Socket::~Socket() {
  this->c.clear_con_listeners();
  this->c.sync_close();
}

void Socket::emit(std::string event, std::string data) {
  Config &config = Config::getInstance();
  config.setIsLoading(1);
  c.socket()->emit(event, data);
}

void Socket::on(std::string event, std::function<void(sio::event &)> callback) {
  c.socket()->on(event, callback);
}

void Socket::auth() {
  Config &config = Config::getInstance();
  Types::User user = config.getUser();
  std::string username =
      user.username.empty() ? "Not authenticated" : user.username;
  emit("authenticate", username);
}

Socket &Socket::getInstance() {
  Config config = Config::getInstance();
  static Socket instance(config.API_URL);
  return instance;
}
