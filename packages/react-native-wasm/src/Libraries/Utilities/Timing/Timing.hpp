#pragma once

#include <cxxreact/CxxModule.h>
#include <mutex>
#include <thread>

using Method = facebook::xplat::module::CxxModule::Method;

namespace ReactNativeWasm {
class Timer {
public:
  Timer(int64_t id, double targetTime, double duration, bool repeat)
    : id(id), targetTime(targetTime), duration(duration), repeat(repeat){};

public:
  int64_t id;
  double targetTime;
  double duration;
  bool repeat;
};

class Timing : public facebook::xplat::module::CxxModule {
public:
  Timing();
  ~Timing();
  std::string getName();
  virtual auto getConstants() -> std::map<std::string, folly::dynamic>;
  virtual auto getMethods() -> std::vector<Method>;

  static const char *Name;

private:
  void tick();
  void loop();

  std::shared_ptr<std::vector<Timer>> timers;
  std::thread thread;
  std::mutex mutex = std::mutex();
};
} // namespace ReactNativeWasm
