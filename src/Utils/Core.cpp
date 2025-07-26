#include "Core.h"
#include <chrono>
#include <thread>

void Utils::Core::sleep(double seconds) {
  int milliseconds = static_cast<int>(seconds * 1000);
  std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}
