#include "NativeQueue.hpp"

#include <iostream>
#include <queue>
#include <mutex>
#include <emscripten/threading.h>

class QueueState {
        public:
            QueueState(
                bool running
            ): running(running) {};

            bool running;
};


// TODO MAJOR UGLY CODE AHEAD, PLEASE VINCENT, REFACTOR THIS HANKEY
namespace ReactNativeWasm {
    auto asyncTasks = std::make_shared<std::queue<std::function<void()>>>();
    auto asyncTasksMutex = std::make_shared<std::mutex>();

    auto syncTasks = std::make_shared<std::queue<std::function<void()>>>();
    auto syncTasksMutex = std::make_shared<std::mutex>();

    auto asyncQueueState = std::make_shared<QueueState>(false);

    void runQueuedTasks(std::shared_ptr<std::queue<std::function<void()>>> tasks, std::shared_ptr<std::mutex> mutex) {
        ReactNativeWasm::asyncQueueState->running = true;
        std::cout << "START QUEUE size:" << std::to_string(tasks->size()) << std::endl;
        // std::lock_guard<std::mutex> guard(*mutex.get());

        while (!tasks->empty()) {
            try {
                std::cout << "START TASK" << std::endl;

                // mutex->lock();
                tasks->front()();

                std::cout << "FINISHED TASK" << std::endl;

                tasks->pop();
                // mutex->unlock();

                std::cout << "UNLOCK MUTEX" << std::endl;
            } catch (...) {
                std::cerr << "Exception running task" << std::endl;
                mutex->unlock();
            }

            std::cout << "FINISHED TASK" << std::endl;
        }

        std::cout << "FINISHED QUEUE" << std::endl;

        ReactNativeWasm::asyncQueueState->running = false;
    };

    void runSyncQueuedTasks() {
        std::cout << "START SYNC QUEUE" << std::endl;
        ReactNativeWasm::runQueuedTasks(syncTasks, syncTasksMutex);
    }

    void runAsyncQueuedTasks() {
        // emscripten_resume_main_loop();

        std::cout << "START ASYNC QUEUE" << std::endl;

        ReactNativeWasm::runQueuedTasks(asyncTasks, asyncTasksMutex);
    }

    NativeQueue::NativeQueue() {}

    void NativeQueue::runOnQueue(std::function<void()> &&func) {
        func();

        // emscripten_dispatch_to_thread_async(
        //     emscripten_main_browser_thread_id(),
        //     EM_FUNC_SIG_VF,
        //     &call_the_function,
        //     0,
        //     &func
        // );

        // asyncTasksMutex->lock();
        // asyncTasks->push(func);
        // asyncTasksMutex->unlock();

        // if (!ReactNativeWasm::asyncQueueState->running) {
        //     ReactNativeWasm::asyncQueueState->running = true;

        //     emscripten_dispatch_to_thread_async(
        //         emscripten_main_browser_thread_id(),
        //         EM_FUNC_SIG_V,
        //         &ReactNativeWasm::runAsyncQueuedTasks,
        //         0
        //     );
        // } else {
        //     std::cout << "ASYNC QUEUE ALREADY RUNNING" << std::endl;
        // }


        // void (*intermediateFuncCall) = [](){};

        // void (*intermediate)(void *) = [intermediateFuncCall](void *eventdispatcher){
        //     // emscripten_async_call(intermediateFuncCall, eventdispatcher, 0);
        // };

        // emscripten_dispatch_to_thread_async(
        //     emscripten_main_browser_thread_id(),
        //     EM_FUNC_SIG_V,
        //     (void *)intermediate,
        //     0
        // );

        // if (emscripten_is_main_browser_thread() == 1) {
        //     func();
        // } else {
        //     std::cout << "BEFORE PUSH ASYNC TASK" << std::endl;

        //     // asyncTasksMutex->lock();
        //     asyncTasks->push(func);
        //     // asyncTasksMutex->unlock();

        //     std::cout << "DISPATCH TASK TO ASYNC" << std::endl;

        //     emscripten_dispatch_to_thread_async(
        //         emscripten_main_browser_thread_id(),
        //         EM_FUNC_SIG_V,
        //         &ReactNativeWasm::runAsyncQueuedTasks,
        //         0
        //     );

        //     std::cout << "FINISHED runOnQueue" << std::endl;
        // }
    }

    void NativeQueue::runOnQueueSync(std::function<void()> &&func) {
        func();

        // if (emscripten_is_main_browser_thread() == 1) {
        //     func();
        // } else {
        //     std::cout << "BEFORE PUSH SYNC TASK" << std::endl;

        //     // syncTasksMutex->lock();
        //     syncTasks->push(func);
        //     // syncTasksMutex->unlock();

        //     std::cout << "DISPATCH TASK TO SYNC" << std::endl;

        //     emscripten_dispatch_to_thread(
        //         emscripten_main_browser_thread_id(),
        //         EM_FUNC_SIG_V,
        //         &ReactNativeWasm::runSyncQueuedTasks,
        //         0
        //     );
        // }

        // func();
    }

    // Once quitSynchronous() returns, no further work should run on the queue.
    void NativeQueue::quitSynchronous() {}
}
