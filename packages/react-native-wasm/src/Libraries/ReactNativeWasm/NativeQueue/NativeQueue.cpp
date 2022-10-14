#include "NativeQueue.hpp"

#include <emscripten/threading.h>
#include <iostream>
#include <mutex>
#include <queue>

// TODO MAJOR UGLY CODE AHEAD, PLEASE VINCENT, REFACTOR THIS HANKEY
namespace ReactNativeWasm {

NativeQueue::NativeQueue() {}

std::vector<std::unique_ptr<Task>> tasks;
std::mutex tasksMutex;

void run(void * func) {
  std::cout << "Starting running a runner" << std::endl;

  auto runnerPtr = static_cast<Task *>(func);
  auto runnerVal = *runnerPtr;

  runnerVal();

  runnerPtr = nullptr;

  std::cout << "Done running a runner" << std::endl;
}

Task NativeQueue::wrapRunnable(Task &&runnable) {
  return [runnable = std::move(runnable)]() mutable {
    if (!runnable) {
      // Runnable is empty, nothing to run.
      return;
    }

    auto localRunnable = std::move(runnable);

    // Clearing `runnable` to free all associated resources that stored lambda
    // might retain.
    runnable = nullptr;

    try {
      localRunnable();
    } catch (std::exception) {
      std::cerr << "An exception occurred while running a function" << std::endl;
    }
  };
}

void NativeQueue::runOnQueue(Task &&func) {
  std::cout << "NativeQueue::runOnQueue" << std::endl;

  if (emscripten_is_main_runtime_thread()) {
    func();

    return;
  }

  auto wrappedRunnable = std::make_unique<Task>(wrapRunnable(std::move(func)));

  emscripten_async_run_in_main_runtime_thread(
    EM_FUNC_SIG_VI, ReactNativeWasm::run, (uint32_t)wrappedRunnable.get());

  std::lock_guard<std::mutex> guard(tasksMutex);

  // TODO: Memory leak ahead
  tasks.push_back(std::move(wrappedRunnable));
}

void NativeQueue::runOnQueueSync(Task &&func) {
  std::cout << "NativeQueue::runOnQueueSync" << std::endl;

  func();
}

// Once quitSynchronous() returns, no further work should run on the queue.
void NativeQueue::quitSynchronous() {}
} // namespace ReactNativeWasm
