#pragma once

#include <sio_client.h>
#include <string>

class Socket {
public:
  Socket(std::string url);
  Socket(Socket &) = delete;
  ~Socket();

  static Socket &getInstance();
  void emit(std::string event, std::string data = "");
  void on(std::string event, std::function<void(sio::event &)> callback);
  void auth();

private:
  sio::client c;
};
