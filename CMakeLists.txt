cmake_minimum_required(VERSION 3.5)
project(
  Mafia
  VERSION 0.0.0
  DESCRIPTION "Online mafia game in C++")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(SOURCE
    ./src/main.cpp
    ./src/Config/Config.cpp
    ./src/Network/Http.cpp
    ./src/Game/Game.cpp
    ./src/Scenes/Menu.cpp
    ./src/Scenes/Rooms.cpp
    ./src/Scenes/CreateRoom.cpp
    ./src/Scenes/Login.cpp
    ./src/Scenes/Register.cpp
    ./src/UI/Menu.cpp
    ./src/Utils/Auth.cpp)
set(ASSETS "${CMAKE_SOURCE_DIR}/assets")

add_compile_definitions(ASSETS="${ASSETS}")

find_package(ftxui REQUIRED)
find_package(CURL REQUIRED)
find_package(sioclient REQUIRED)

add_executable(mafia ${SOURCE})
target_link_libraries(mafia PRIVATE ftxui::screen ftxui::dom ftxui::component
                                    CURL::libcurl sioclient)
