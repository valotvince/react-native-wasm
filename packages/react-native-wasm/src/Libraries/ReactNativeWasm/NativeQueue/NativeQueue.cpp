#include "NativeQueue.hpp"

namespace ReactNativeWasm {
    NativeQueue::NativeQueue() {}

    void NativeQueue::runOnQueue(std::function<void()> &&func) {
        func();
    }

    void NativeQueue::runOnQueueSync(std::function<void()> &&func) {
        func();
    }

    // Once quitSynchronous() returns, no further work should run on the queue.
    void NativeQueue::quitSynchronous() {}
}
