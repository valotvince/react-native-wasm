
#include "PlatformConstants.hpp"
#include <cxxreact/ReactNativeVersion.h>

using Method = facebook::xplat::module::CxxModule::Method;

namespace facebook::react {
const char *PlatformConstantsModule::Name = "PlatformConstants";

std::string PlatformConstantsModule::getName() { return PlatformConstantsModule::Name; }

std::vector<Method> PlatformConstantsModule::getMethods() { return {}; }

std::map<std::string, folly::dynamic> PlatformConstantsModule::getConstants() {
  return {
    {"isTesting", false},
    {"reactNativeVersion",
     folly::dynamic::object("major", ReactNativeVersion.Major)("minor", ReactNativeVersion.Minor)(
       "patch", ReactNativeVersion.Patch)("prerelease", StringOrNull(ReactNativeVersion.Prerelease))},
    {"osVersion", "na"},
  };
}

folly::dynamic PlatformConstantsModule::StringOrNull(std::string_view str) noexcept {
  return str.empty() ? folly::dynamic(nullptr) : folly::dynamic(str);
}
} // namespace facebook::react
