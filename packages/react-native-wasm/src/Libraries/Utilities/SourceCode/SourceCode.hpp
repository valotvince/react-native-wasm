#pragma once

#include <cxxreact/CxxModule.h>
#include <mutex>
#include <thread>

using Method = facebook::xplat::module::CxxModule::Method;

namespace ReactNativeWasm {
class SourceCode : public facebook::xplat::module::CxxModule {
public:
  SourceCode();
  ~SourceCode();
  std::string getName();
  virtual auto getConstants() -> std::map<std::string, folly::dynamic>;
  virtual auto getMethods() -> std::vector<Method>;
};
} // namespace ReactNativeWasm
