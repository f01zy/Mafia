#include "Socket.h"
#include "../Config/Config.h"
#include "../Config/State.h"
#include "../Types/JsonSerializers.h"
#include "../Utils/Json.h"

State &state = State::getInstance();
Types::Room emptyRoom;

void receiveStartGame(sio::event event) {
  state.setRoom(emptyRoom);
  state.screen.Exit();
  state.scene = Types::Scene::Game;
  state.isLoading = false;
}

void error(sio::event event) {
  std::string data = event.get_message()->get_string();
  state.error = data;
  state.isLoading = false;
}

void updateRoom(sio::event event) {
  json data = json::parse(event.get_message()->get_string());
  Types::Room room = Utils::Json::jsonToRoom(data);
  state.setRoom(room);
  state.isRoomUpdated = true;
  state.screen.Exit();
  state.isLoading = false;
}

void receiveRooms(sio::event event) {
  json data = json::parse(event.get_message()->get_string());
  state.rooms = data["rooms"].get<std::vector<Types::Room>>();
  state.isLoading = false;
}

void receiveDisconnectFromRoom(sio::event event) {
  state.setRoom(emptyRoom);
  state.isLoading = false;
}

Socket::Socket(std::string url) {
  setupEvents();
  c.set_logs_quiet();
  c.connect(url);
}

Socket::~Socket() {
  this->c.clear_con_listeners();
  this->c.sync_close();
}

void Socket::emit(std::string event, std::string data) {
  state.isLoading = true;
  c.socket()->emit(event, data);
}

void Socket::on(std::string event, std::function<void(sio::event &)> callback) {
  c.socket()->on(event, callback);
}

void Socket::auth() {
  Types::User user = state.getUser();
  emit("authenticate", user.username);
  state.isLoading = false;
}

void Socket::setupEvents() {
  on("receiveDisconnectFromRoom", receiveDisconnectFromRoom);
  on("updateRoom", updateRoom);
  on("receiveRooms", receiveRooms);
  on("error", error);
  on("receiveStartGame", receiveStartGame);
}

Socket &Socket::getInstance() {
  Config config = Config::getInstance();
  static Socket instance(config.API_URL);
  return instance;
}
