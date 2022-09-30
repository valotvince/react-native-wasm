#include <chrono>
#include <folly/dynamic.h>
#include <iostream>
#include <thread>

#include "Timing.hpp"

#include <cxxreact/Instance.h>
#include <cxxreact/JsArgumentHelpers.h>

namespace ReactNativeWasm {
Timing::Timing() {
  timers = std::make_shared<std::vector<Timer>>();
  thread = std::thread(&Timing::loop, std::ref(*this));
}

Timing::~Timing() { pthread_cancel(thread.native_handle()); }

const char *Timing::Name = "Timing";

std::string Timing::getName() { return Timing::Name; }

auto Timing::getConstants() -> std::map<std::string, folly::dynamic> { return {}; }

void Timing::tick() {
  folly::dynamic timersToFire = folly::dynamic::array();
  auto nextTimers = std::vector<Timer>();

  mutex.lock();

  for (auto it = timers->begin(); it != timers->end();) {
    auto timer = it;

    auto now = std::chrono::system_clock::now().time_since_epoch().count();

    if (now >= timer->targetTime) {
      timersToFire.push_back(timer->id);

      if (timer->repeat) {
        auto nextTimer = Timer(timer->id, now + timer->duration, timer->duration, timer->repeat);

        nextTimers.push_back(nextTimer);
      }

      it = timers->erase(it);
    } else {
      ++it;
    }
  }

  for (auto it = nextTimers.begin(); it != nextTimers.end(); ++it) {
    timers->push_back(nextTimers.at(std::distance(nextTimers.begin(), it)));
  }

  mutex.unlock();

  if (!timersToFire.empty()) {
    auto instance = getInstance().lock();

    if (instance) {
      auto args = folly::dynamic::array(timersToFire);

      instance->callJSFunction("JSTimers", "callTimers", std::move(args));
    } else {
      std::cerr << "not locked" << std::endl;
    }
  }
}

void Timing::loop() {
  while (true) {
    try {
      this->tick();
    } catch (const std::exception &e) {
      std::cerr << "Exception " << e.what() << std::endl;
    } catch (...) {
      std::cerr << "Exception raised in tick" << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}

auto Timing::getMethods() -> std::vector<Method> {
  return {
    Method(
      "createTimer",
      [this](folly::dynamic args) {
        auto now = std::chrono::system_clock::now().time_since_epoch().count();

        auto id = facebook::xplat::jsArgAsInt(args, 0);
        auto duration = facebook::xplat::jsArgAsDouble(args, 1);
        auto jsSchedulingTime = facebook::xplat::jsArgAsDouble(args, 2);
        auto repeat = facebook::xplat::jsArgAsBool(args, 3);

        auto targetTime = now + duration;

        // int64_t id, double duration, double jsSchedulingTime, bool repeat
        // Timer timer = Timer(id, targetTime, repeat);
        std::lock_guard<std::mutex> guard(mutex);

        this->timers->push_back(Timer(id, targetTime, duration, repeat));
      }),
    Method(
      "deleteTimer",
      [this](folly::dynamic args) {
        auto deleteTimerId = facebook::xplat::jsArgAsInt(args, 0);

        std::lock_guard<std::mutex> guard(mutex);

        for (auto it = this->timers->begin(); it != timers->end(); ++it) {
          if (it->id == deleteTimerId) {
            std::cout << "Timing > Found timer to erase" << std::endl;
            this->timers->erase(it);
            break;
          }
        }
      }),
    Method("setSendIdleEvents", [](folly::dynamic args) {}),
  };
}
} // namespace ReactNativeWasm
