#pragma once

#include "../Types/JsonSerializers.h"
#include "../Types/Types.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace Utils::Json {
Types::Room jsonToRoom(json data);
Types::User jsonToUser(json data);
} // namespace Utils::Json
