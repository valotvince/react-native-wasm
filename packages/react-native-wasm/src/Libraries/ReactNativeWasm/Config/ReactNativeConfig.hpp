#pragma once

#include <react/config/ReactNativeConfig.h>
#include <folly/dynamic.h>

namespace ReactNativeWasm {
    class ReactNativeConfig : public facebook::react::ReactNativeConfig {
        public:
            ReactNativeConfig(folly::dynamic store): store(store) {};
            ~ReactNativeConfig() override;

            bool getBool(const std::string &param) const override;
            std::string getString(const std::string &param) const override;
            int64_t getInt64(const std::string &param) const override;
            double getDouble(const std::string &param) const override;

        private:
            folly::dynamic store;
    };
}
