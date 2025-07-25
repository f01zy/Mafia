#pragma once

#include <string>

class Config final {
public:
  static Config &getInstance() {
    static Config config;
    return config;
  };

  const unsigned WIDTH = 600, HEIGHT = 600;
  const std::string API_URL = "http://localhost:5000";
};
