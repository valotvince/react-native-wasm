
#include <iostream>
#include <SDL2/SDL.h>

#include <cxxreact/NativeModule.h>
#include <cxxreact/CxxNativeModule.h>
#include <cxxreact/ModuleRegistry.h>

#include "Libraries/Utilities/PlatformConstants.hpp"
#include "Libraries/Components/View/View.hpp"
#include "Libraries/ReactNativeWasm/Renderer/Renderer.hpp"
#include "ReactWasmInstance.hpp"
#include "Libraries/ReactNativeWasm/NativeQueue/NativeQueue.hpp"

using SharedNativeModuleVector = std::vector<std::shared_ptr<facebook::react::NativeModule>>;
using UniqueNativeModuleVector = std::vector<std::unique_ptr<facebook::react::NativeModule>>;

UniqueNativeModuleVector getNativeModules(std::shared_ptr<ReactNativeWasm::Instance> instance, std::shared_ptr<ReactNativeWasm::NativeQueue> nativeQueue) {
    UniqueNativeModuleVector modules;

    modules.push_back(
        std::make_unique<facebook::react::CxxNativeModule>(
            instance,
            facebook::react::PlatformConstantsModule::Name,
            []() { return std::make_unique<facebook::react::PlatformConstantsModule>(); },
            nativeQueue
        )
    );

    // modules.push_back(
    //     std::make_unique<facebook::react::CxxNativeModule>(
    //         instance,
    //         "RNWView",
    //         []() { return std::make_unique<ReactNativeWasm::Components::View>(); },
    //         nativeQueue
    //     )
    // );

    return modules;
}

SharedNativeModuleVector getSharedNativeModules(std::shared_ptr<ReactNativeWasm::Instance> instance, std::shared_ptr<ReactNativeWasm::NativeQueue> nativeQueue) {
    SharedNativeModuleVector modules;

    modules.push_back(
        std::make_shared<facebook::react::CxxNativeModule>(
            instance,
            facebook::react::PlatformConstantsModule::Name,
            []() { return std::make_unique<facebook::react::PlatformConstantsModule>(); },
            nativeQueue
        )
    );

    // modules.push_back(
    //     std::make_unique<facebook::react::CxxNativeModule>(
    //         instance,
    //         "RNWView",
    //         []() { return std::make_unique<ReactNativeWasm::Components::View>(); },
    //         nativeQueue
    //     )
    // );

    return modules;
}

int main(int argc, char* argv[]) {
    auto instance = std::make_shared<ReactNativeWasm::Instance>();
    auto nativeQueue = std::make_shared<ReactNativeWasm::NativeQueue>();

    auto moduleRegistry = std::make_shared<facebook::react::ModuleRegistry>(std::move(getNativeModules(instance, nativeQueue)));

    auto sharedModules = std::make_shared<SharedNativeModuleVector>(getSharedNativeModules(instance, nativeQueue));

    instance->setupRuntime(moduleRegistry, std::move(sharedModules));
    instance->loadBundle();

    std::cout << "Hello World" << std::endl;
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

    return 0;
}
