#pragma once

#include "../ComponentManager.hpp"

#include <folly/dynamic.h>

namespace ReactNativeWasm::Components {
    class TextManager : public Manager {
        public:
            TextManager();

            std::string getName() override;
            folly::dynamic getConstants() override;
            void createView() override;

            static const char *Name;
    };
}
