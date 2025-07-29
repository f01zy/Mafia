#include "Utils.h"
#include "../Config/State.h"
#include <chrono>
#include <thread>

void Utils::sleep(double seconds) {
  int milliseconds = static_cast<int>(seconds * 1000);
  std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void Utils::loading() {
  State &state = State::getInstance();

  while (state.isLoading) {
    Utils::sleep(0.5);
  }
}
