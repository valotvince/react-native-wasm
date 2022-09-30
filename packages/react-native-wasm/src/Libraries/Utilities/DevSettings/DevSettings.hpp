#pragma once

#include <cxxreact/CxxModule.h>
#include <string.h>

namespace ReactNativeWasm {
class DevSettings : public facebook::xplat::module::CxxModule {
public:
  void Initialize() noexcept;
  void reload() noexcept;
  void reloadWithReason(std::string) noexcept;
  void onFastRefresh() noexcept;
  void setHotLoadingEnabled(bool isHotLoadingEnabled) noexcept;
  void setIsDebuggingRemotely(bool isDebuggingRemotelyEnabled) noexcept;
  void setProfilingEnabled(bool isProfilingEnabled) noexcept;
  void toggleElementInspector() noexcept;
  void addMenuItem(std::string title) noexcept;
  void setIsShakeToShowDevMenuEnabled(bool enabled) noexcept;
  void addListener(std::string eventName) noexcept;
  void removeListeners(double count) noexcept;

  std::string getName();
  std::map<std::string, folly::dynamic> getConstants();
  std::vector<Method> getMethods();
};
} // namespace ReactNativeWasm
