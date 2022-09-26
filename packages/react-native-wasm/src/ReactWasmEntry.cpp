
#include <iostream>
#include <SDL2/SDL.h>
#include <thread>
#include <chrono>
#include <emscripten/emscripten.h>
#include <emscripten/bind.h>

#include <cxxreact/NativeModule.h>
#include <cxxreact/CxxNativeModule.h>
#include <cxxreact/ModuleRegistry.h>
#include <cxxreact/Instance.h>
#include <folly/dynamic.h>

#include "Libraries/Utilities/DevSettings/DevSettings.hpp"
#include "Libraries/Utilities/PlatformConstants.hpp"
#include "Libraries/Utilities/Timing/Timing.hpp"

#include "Libraries/Components/View/View.hpp"
#include "Libraries/ReactNativeWasm/Bindings/JSWasmExecutor.hpp"
#include "Libraries/ReactNativeWasm/Renderer/Renderer.hpp"
#include "ReactWasmInstance.hpp"
#include "Libraries/ReactNativeWasm/NativeQueue/NativeQueue.hpp"

using SharedNativeModuleVector = std::vector<std::shared_ptr<facebook::react::NativeModule>>;
using UniqueNativeModuleVector = std::vector<std::unique_ptr<facebook::react::NativeModule>>;

UniqueNativeModuleVector getNativeModules(std::shared_ptr<facebook::react::Instance> instance, std::shared_ptr<ReactNativeWasm::NativeQueue> nativeQueue) {
    UniqueNativeModuleVector modules;

    modules.push_back(
        std::make_unique<facebook::react::CxxNativeModule>(
            instance,
            facebook::react::PlatformConstantsModule::Name,
            []() { return std::make_unique<facebook::react::PlatformConstantsModule>(); },
            nativeQueue
        )
    );

    modules.push_back(
        std::make_unique<facebook::react::CxxNativeModule>(
            instance,
            "DevSettings",
            []() { return std::make_unique<ReactNativeWasm::DevSettings>(); },
            nativeQueue
        )
    );

    modules.push_back(
        std::make_unique<facebook::react::CxxNativeModule>(
            instance,
            ReactNativeWasm::Timing::Name,
            []() { return std::make_unique<ReactNativeWasm::Timing>(); },
            nativeQueue
        )
    );

    return modules;
}

SharedNativeModuleVector getSharedNativeModules(std::shared_ptr<facebook::react::Instance> instance, std::shared_ptr<ReactNativeWasm::NativeQueue> nativeQueue) {
    SharedNativeModuleVector modules;

    modules.push_back(
        std::make_shared<facebook::react::CxxNativeModule>(
            instance,
            facebook::react::PlatformConstantsModule::Name,
            []() { return std::make_unique<facebook::react::PlatformConstantsModule>(); },
            nativeQueue
        )
    );

    modules.push_back(
        std::make_shared<facebook::react::CxxNativeModule>(
            instance,
            "DevSettings",
            []() { return std::make_unique<ReactNativeWasm::DevSettings>(); },
            nativeQueue
        )
    );

    modules.push_back(
        std::make_shared<facebook::react::CxxNativeModule>(
            instance,
            ReactNativeWasm::Timing::Name,
            []() { return std::make_unique<ReactNativeWasm::Timing>(); },
            nativeQueue
        )
    );

    return modules;
}

std::shared_ptr<facebook::react::Instance> reactInstance;
std::shared_ptr<ReactNativeWasm::Instance> instance;
std::shared_ptr<ReactNativeWasm::NativeQueue> nativeQueue;

struct InstanceCallback : public facebook::react::InstanceCallback {
  ~InstanceCallback() override {}
  void onBatchComplete() override {}
  void incrementPendingJSCalls() override {}
  void decrementPendingJSCalls() override {}
};

void run() {
    instance = std::make_shared<ReactNativeWasm::Instance>();
    nativeQueue = std::make_shared<ReactNativeWasm::NativeQueue>();

    reactInstance = std::make_shared<facebook::react::Instance>();

    auto moduleRegistry = std::make_shared<facebook::react::ModuleRegistry>(std::move(getNativeModules(reactInstance, nativeQueue)));
    auto sharedModules = std::make_shared<SharedNativeModuleVector>(getSharedNativeModules(reactInstance, nativeQueue));

    reactInstance->initializeBridge(
        std::make_unique<InstanceCallback>(),
        std::make_shared<ReactNativeWasm::JSWasmExecutorFactory>(),
        nativeQueue,
        moduleRegistry
    );

    instance->setupRuntime(moduleRegistry, std::move(sharedModules));
    instance->loadBundle();

    std::cout << "After loadBundle" << std::endl;

    // while (true) {}
}

int main(int argc, char* argv[]) {
    // auto thread = std::thread(&run);
    // emscripten_set_main_loop(&run, 0, true);

    run();

    std::cout << "Hello World 1" << std::endl;
    // std::cout << "The time from Epoch is: " << helloworld::time() << std::endl;

    // Initialize SDL graphics subsystem.
    SDL_Init(SDL_INIT_VIDEO);

    // Initialize a 300x300 window and a renderer.
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_CreateWindowAndRenderer(300, 300, 0, &window, &renderer);

    ReactNativeWasm::Renderer::setRenderer(&renderer);

    // Set a color for drawing matching the earlier `ctx.fillStyle = "green"`.
    SDL_SetRenderDrawColor(renderer, /* RGBA: green */ 0x00, 0x80, 0x00, 0xFF);
    // Create and draw a rectangle like in the earlier `ctx.fillRect()`.
    SDL_Rect rect = {.x = 10, .y = 10, .w = 10, .h = 10};
    SDL_RenderFillRect(renderer, &rect);

    // Render everything from a buffer to the actual screen.
    SDL_RenderPresent(renderer);

    emscripten_exit_with_live_runtime();

    return 0;
}

void onBundleLoaded() {
    folly::dynamic params = folly::dynamic::array(
    std::move("main"),
    folly::dynamic::object("initialProps", {})("rootTag", 11)(
        "fabric", false));

    reactInstance->callJSFunction("AppRegistry", "runApplication", std::move(params));
}

EMSCRIPTEN_BINDINGS(ReactWasmInstance) {
    emscripten::function("__onBundleLoaded", &onBundleLoaded);
}
