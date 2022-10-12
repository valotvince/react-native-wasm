#pragma once

#include <cxxreact/CxxModule.h>

using Method = facebook::xplat::module::CxxModule::Method;

namespace ReactNativeWasm {

class StatusBarManager : public facebook::xplat::module::CxxModule {
public:
  std::string getName() override;
  std::map<std::string, folly::dynamic> getConstants() override;
  std::vector<Method> getMethods() override;
};

} // namespace ReactNativeWasm
