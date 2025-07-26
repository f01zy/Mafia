#include "../Types/Types.h"
#include <string>

namespace Scenes {
Types::Scene Menu();
Types::Scene Rooms();
Types::Scene Login(std::string title = "Login");
Types::Scene Register(std::string title = "Register");
} // namespace Scenes
