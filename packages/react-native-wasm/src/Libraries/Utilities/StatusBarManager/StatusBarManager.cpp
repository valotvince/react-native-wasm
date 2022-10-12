#include "StatusBarManager.hpp"

namespace ReactNativeWasm {

std::string StatusBarManager::getName() { return "StatusBarManager"; }

std::vector<Method> StatusBarManager::getMethods() {
  return {
    Method("setStyle", [](folly::dynamic /*args*/) noexcept {}),
    Method("setHidden", [](folly::dynamic /*args*/) noexcept {}),
  };
}

std::map<std::string, folly::dynamic> StatusBarManager::getConstants() { return {{"HEIGHT", 0}}; }

} // namespace ReactNativeWasm
