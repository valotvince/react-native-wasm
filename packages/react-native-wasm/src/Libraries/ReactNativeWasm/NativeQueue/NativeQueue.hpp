#pragma once

#include <cxxreact/MessageQueueThread.h>
#include <functional>
#include <queue>

namespace ReactNativeWasm {
class NativeQueue : public facebook::react::MessageQueueThread {
public:
  NativeQueue();

  void runOnQueue(std::function<void()> &&) override;
  // runOnQueueSync and quitSynchronous are dangerous.  They should only be
  // used for initialization and cleanup.
  void runOnQueueSync(std::function<void()> &&) override;
  // Once quitSynchronous() returns, no further work should run on the queue.
  void quitSynchronous() override;
};
} // namespace ReactNativeWasm
