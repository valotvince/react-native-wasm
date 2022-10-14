#pragma once

#include <cxxreact/MessageQueueThread.h>
#include <functional>
#include <queue>

namespace ReactNativeWasm {
using Task = std::function<void()>;
class NativeQueue : public facebook::react::MessageQueueThread {
public:
  NativeQueue();

  void runOnQueue(Task &&) override;
  // runOnQueueSync and quitSynchronous are dangerous.  They should only be
  // used for initialization and cleanup.
  void runOnQueueSync(Task &&) override;
  // Once quitSynchronous() returns, no further work should run on the queue.
  void quitSynchronous() override;

private:
  Task wrapRunnable(Task &&);
};
} // namespace ReactNativeWasm
