#pragma once

#include <string>

class Http final {
public:
  std::string request(std::string endpoint, std::string data = "");

private:
  static size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                              void *userp);
};
