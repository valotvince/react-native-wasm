#include "DeviceInfo.hpp"

namespace ReactNativeWasm {
DeviceInfo::DeviceInfo(){};
DeviceInfo::~DeviceInfo(){};

auto DeviceInfo::getMethods() -> std::vector<Method> {
  return {
    // Method(
    //   "getConstants",
    //   [this](folly::dynamic args) {

    //   }
    // )
  };
}

std::string DeviceInfo::getName() { return "DeviceInfo"; }

auto DeviceInfo::getConstants() -> std::map<std::string, folly::dynamic> {
  folly::dynamic dimensions = folly::dynamic::object("window", folly::dynamic::object("width", 300)("height", 300))(
    "screen", folly::dynamic::object("width", 300)("height", 300));

  return {{"Dimensions", dimensions}};
}
} // namespace ReactNativeWasm
