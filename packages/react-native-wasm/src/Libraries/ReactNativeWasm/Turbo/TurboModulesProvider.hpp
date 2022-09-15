#pragma once

#include <ReactCommon/LongLivedObject.h>
#include "./TurboModuleRegistry.hpp"

  interface IReactModuleBuilder
  {
    DOC_STRING(
      "Adds an initializer method called on the native module initialization.\n"
      "It provides the native module with the @IReactContext for the running ReactNative instance. "
      "See @InitializerDelegate.\n"
      "There can be multiple initializer methods which are called in the order they were registered.")
    void AddInitializer(InitializerDelegate initializer);

    DOC_STRING(
      "Adds a constant provider method to define constants for the native module. See @ConstantProviderDelegate.")
    void AddConstantProvider(ConstantProviderDelegate constantProvider);

    DOC_STRING("Adds an asynchronous method to the native module. See @MethodDelegate.")
    void AddMethod(String name, MethodReturnType returnType, MethodDelegate method);

    DOC_STRING("Adds a synchronous method to the native module. See @SyncMethodDelegate.")
    void AddSyncMethod(String name, SyncMethodDelegate method);
  };

namespace ReactNativeWasm::Turbo {

    class TurboModulesProvider final : public facebook::react::TurboModuleRegistry {
    public: // TurboModuleRegistry implementation
    std::shared_ptr<facebook::react::TurboModule> getModule(
        const std::string &moduleName,
        const std::shared_ptr<facebook::react::CallInvoker> &callInvoker) noexcept override;
    std::vector<std::string> getEagerInitModuleNames() noexcept override;

    public:
    //   void SetReactContext(const IReactCfontext &reactContext) noexcept;
    // void AddModuleProvider(
    //     std::string const &moduleName,
    //     ReactModuleProvider const &moduleProvider,
    //     bool overwriteExisting) noexcept;
    std::shared_ptr<facebook::react::LongLivedObjectCollection> const &LongLivedObjectCollection() noexcept;

    private:
    // To keep a list of deferred asynchronous callbacks and promises.
    std::shared_ptr<facebook::react::LongLivedObjectCollection> m_longLivedObjectCollection{
        std::make_shared<facebook::react::LongLivedObjectCollection>()};
    std::unordered_map<std::string, ReactModuleProvider> m_moduleProviders;
    // IReactContext m_reactContext;
    };
}
