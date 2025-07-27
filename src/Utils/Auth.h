#pragma once

#include <string>

namespace Utils::Auth {
bool checkAuth();
std::string signIn(std::string username, std::string password);
std::string signUp(std::string username, std::string email,
                   std::string password);
std::string refresh();
void logout();
std::string callback(std::string res);
void setToken(std::string token);
std::string getToken();
} // namespace Utils::Auth
