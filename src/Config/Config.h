#pragma once

#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

class Config final {
public:
  static Config &getInstance() {
    static Config config;
    return config;
  };

  const unsigned WIDTH = 600, HEIGHT = 600;
  const std::string API_URL = "http://localhost:3000";
};
