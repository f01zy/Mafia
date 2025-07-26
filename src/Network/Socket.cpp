#include "Socket.h"
#include "../Config/Config.h"
#include "../Utils/Core.h"

Socket::Socket(std::string url) {
  Utils::Core::sleep(1);
  c.set_logs_quiet();
  c.connect(url);
}

Socket::~Socket() {
  this->c.clear_con_listeners();
  this->c.sync_close();
}

void Socket::send(std::string event, std::string data) {
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
