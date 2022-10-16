#pragma once

#include "../../Components/ComponentManager.hpp"
#include <cxxreact/CxxModule.h>
#include <folly/dynamic.h>
#include <map>
#include <react/renderer/uimanager/UIManager.h>

using Method = facebook::xplat::module::CxxModule::Method;

namespace ReactNativeWasm {
class NativeAnimatedModule : public facebook::xplat::module::CxxModule {
public:
  NativeAnimatedModule(){};

  std::string getName();
  auto getConstants() -> std::map<std::string, folly::dynamic>;
  auto getMethods() -> std::vector<Method>;

  static const char *Name;
};
} // namespace ReactNativeWasm
