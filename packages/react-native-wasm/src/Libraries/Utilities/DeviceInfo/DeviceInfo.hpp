#pragma once

#include <cxxreact/CxxModule.h>
#include <mutex>
#include <thread>

using Method = facebook::xplat::module::CxxModule::Method;

namespace ReactNativeWasm {
class DeviceInfo : public facebook::xplat::module::CxxModule {
public:
  DeviceInfo();
  ~DeviceInfo();
  std::string getName();
  virtual auto getConstants() -> std::map<std::string, folly::dynamic>;
  virtual auto getMethods() -> std::vector<Method>;
};
} // namespace ReactNativeWasm
