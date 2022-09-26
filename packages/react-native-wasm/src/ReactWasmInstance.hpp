#pragma once

#include <cxxreact/ModuleRegistry.h>
#include <ReactCommon/LongLivedObject.h>
#include <cxxreact/CxxNativeModule.h>

namespace ReactNativeWasm {
    class Instance {
        public:
            ~Instance();

            void loadBundle();
            void setupRuntime(std::shared_ptr<facebook::react::ModuleRegistry>, std::shared_ptr<std::vector<std::shared_ptr<facebook::react::NativeModule>>> modules);
            void startRender();
    };
}
