#include "SourceCode.hpp"

namespace ReactNativeWasm {
SourceCode::SourceCode(){};
SourceCode::~SourceCode(){};

auto SourceCode::getMethods() -> std::vector<Method> { return {}; }

std::string SourceCode::getName() { return "SourceCode"; }

auto SourceCode::getConstants() -> std::map<std::string, folly::dynamic> {
  return {{"scriptUrl", "react-native.bundle.js"}};
}
} // namespace ReactNativeWasm
