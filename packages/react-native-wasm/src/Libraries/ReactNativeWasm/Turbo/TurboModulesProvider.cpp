// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//
// IMPORTANT: Before updating this file
// please read react-native-windows repo:
// vnext/Microsoft.ReactNative.Cxx/README.md

#include "TurboModulesProvider.hpp"
#include <ReactCommon/TurboModuleUtils.h>

namespace ReactNativeWasm::Turbo {
  std::shared_ptr<facebook::react::TurboModule> TurboModulesProvider::getModule(
      const std::string &moduleName,
      const std::shared_ptr<facebook::react::CallInvoker> &callInvoker) noexcept {
    // fail if the expected turbo module has not been registered
    auto it = m_moduleProviders.find(moduleName);
    if (it == m_moduleProviders.end()) {
      return nullptr;
    }

    // auto tm = std::make_shared<TurboModuleImpl>(
    //     m_reactContext, moduleName, callInvoker, m_longLivedObjectCollection, /*reactModuleProvider*/ it->second);
    return it->second;
  }

  std::vector<std::string> TurboModulesProvider::getEagerInitModuleNames() noexcept {
    std::vector<std::string> eagerModules;
    auto it = m_moduleProviders.find("UIManager");
    if (it != m_moduleProviders.end()) {
      eagerModules.push_back("UIManager");
    }
    return eagerModules;
  }

  // void TurboModulesProvider::SetReactContext(const IReactContext &reactContext) noexcept {
  //   m_reactContext = reactContext;
  // }

  void TurboModulesProvider::AddModuleProvider(
      std::string const moduleName,
      ReactModuleProvider const &moduleProvider,
      bool overwriteExisting) noexcept {
    auto it = m_moduleProviders.find(moduleName);
    if (it == m_moduleProviders.end()) {
      m_moduleProviders.insert({moduleName, moduleProvider});
    } else if (overwriteExisting) {
      // turbo modules should be replaceable before the first time it is requested
      it->second = moduleProvider;
    }
  }

  std::shared_ptr<facebook::react::LongLivedObjectCollection> const &
  TurboModulesProvider::LongLivedObjectCollection() noexcept {
    return m_longLivedObjectCollection;
  }
}
