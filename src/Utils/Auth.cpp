#include "Auth.h"
#include "../Config/Config.h"
#include "../Network/Http.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

bool Utils::Auth::checkAuth() {
  std::string isAuth = refresh();

  if (isAuth.empty()) {
    return true;
  }

  return false;
}

std::string Utils::Auth::signIn(std::string username, std::string password) {
  Http http;
  std::string fields = "username=" + username + "&password=" + password;
  std::string res = http.request("/auth/login", fields);
  return callback(res);
}

std::string Utils::Auth::signUp(std::string username, std::string email,
                                std::string password) {
  Http http;
  std::string fields =
      "&username=" + username + "&email=" + email + "&password=" + password;
  std::string res = http.request("/auth/register", fields);
  return callback(res);
}

std::string Utils::Auth::refresh() {
  Http http;
  std::string refreshToken = getToken();
  std::string fields = "refreshToken=" + refreshToken;
  std::string res = http.request("/auth/refresh", fields);
  return callback(res);
}

void Utils::Auth::logout() {
  Http http;
  std::string refreshToken = getToken();
  std::string fields = "refreshToken=" + refreshToken;
  http.request("/auth/logout", fields);
  remove("token.txt");
}

std::string Utils::Auth::callback(std::string res) {
  try {
    json data = json::parse(res);

    if (data.contains("refreshToken")) {
      setToken(data["refreshToken"]);
      Config::user = data["user"];
      return "";
    }

    return data["message"];
  }

  catch (const json::parse_error &e) {
    return "Response parse error";
  }
}

void Utils::Auth::setToken(std::string token) {
  std::ofstream file("token");

  if (file) {
    file << token << std::endl;
  }
}

std::string Utils::Auth::getToken() {
  std::string token;
  std::ifstream file("token");

  if (file.is_open()) {
    getline(file, token);
    file.close();
  }

  return token;
}
