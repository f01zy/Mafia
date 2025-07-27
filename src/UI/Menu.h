#pragma once

#include <string>
#include <vector>

namespace UI {
class Menu {
public:
  Menu(std::vector<std::string> options, std::string title)
      : options(options), title(title) {};
  Menu(Menu &) = delete;
  int draw();

private:
  std::vector<std::string> options;
  std::string title;
};
} // namespace UI
