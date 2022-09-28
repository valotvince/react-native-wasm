#pragma once

#include <folly/dynamic.h>

namespace ReactNativeWasm::Components {
    class Manager {
        public:
            virtual std::string getName() = 0;
            virtual folly::dynamic getConstants() = 0;
            virtual void createView() = 0;

            static const char *Name;
    };
}
