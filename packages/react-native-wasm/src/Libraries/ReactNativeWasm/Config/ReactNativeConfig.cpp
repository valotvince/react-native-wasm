#include "ReactNativeConfig.hpp"

namespace ReactNativeWasm {
    ReactNativeConfig::~ReactNativeConfig() {};

    bool ReactNativeConfig::getBool(const std::string &param) const {
        return store.at(param).asBool();
    };
    std::string ReactNativeConfig::getString(const std::string &param) const {
        return store.at(param).asString();
    };
    int64_t ReactNativeConfig::getInt64(const std::string &param) const {
        return store.at(param).asInt();
    };
    double ReactNativeConfig::getDouble(const std::string &param) const {
        return store.at(param).asDouble();
    };
}
