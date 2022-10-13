#include "NativeQueue.hpp"

#include <emscripten/threading.h>
#include <iostream>
#include <mutex>
#include <queue>

// TODO MAJOR UGLY CODE AHEAD, PLEASE VINCENT, REFACTOR THIS HANKEY
namespace ReactNativeWasm {

NativeQueue::NativeQueue() {}

void NativeQueue::runOnQueue(std::function<void()> &&func) {
  std::cout << "NativeQueue::runOnQueue" << std::endl;
  func();
}

void NativeQueue::runOnQueueSync(std::function<void()> &&func) {
  std::cout << "NativeQueue::runOnQueueSync" << std::endl;
  func();
}

// Once quitSynchronous() returns, no further work should run on the queue.
void NativeQueue::quitSynchronous() {}
} // namespace ReactNativeWasm
