#include "JSWasmExecutor.hpp"
#include "../../Utilities/JavascriptAccessor/JavascriptAccessor.hpp"

#include <iostream>

#include <folly/json.h>

namespace ReactNativeWasm {
std::unique_ptr<facebook::react::JSExecutor> JSWasmExecutorFactory::createJSExecutor(
  std::shared_ptr<facebook::react::ExecutorDelegate> delegate,
  std::shared_ptr<facebook::react::MessageQueueThread> jsQueue) {
  return std::make_unique<JSWasmExecutor>(delegate, jsQueue);
}

/**
 * Prepares the JS runtime for React Native by installing global variables.
 * Called once before any JS is evaluated.
 */
void JSWasmExecutor::initializeRuntime() {
  std::cout << "initializeRuntime" << std::endl;
  folly::dynamic nativeModuleConfig = folly::dynamic::array;

  auto moduleRegistry = delegate->getModuleRegistry();
  for (const auto &name : moduleRegistry->moduleNames()) {
    auto config = moduleRegistry->getConfig(name);
    nativeModuleConfig.push_back(config ? config->config : nullptr);
  }

  folly::dynamic config = folly::dynamic::object("remoteModuleConfig", std::move(nativeModuleConfig));

  setGlobalVariable(
    "__fbBatchedBridgeConfig", std::make_unique<facebook::react::JSBigStdString>(folly::toJson(config)));
}

/**
 * Execute an application script bundle in the JS context.
 */
void JSWasmExecutor::loadBundle(std::unique_ptr<const facebook::react::JSBigString> script, std::string sourceURL) {
  std::cout << "loadBundle not implemented" << std::endl;
}

/**
 * Add an application "RAM" bundle registry
 */
void JSWasmExecutor::setBundleRegistry(std::unique_ptr<facebook::react::RAMBundleRegistry> bundleRegistry) {
  std::cout << "setBundleRegistry not implemented" << std::endl;
}

/**
 * Register a file path for an additional "RAM" bundle
 */
void JSWasmExecutor::registerBundle(uint32_t bundleId, const std::string &bundlePath) {
  std::cout << "registerBundle not implemented" << std::endl;
}

/**
 * Executes BatchedBridge.callFunctionReturnFlushedQueue with the module ID,
 * method ID and optional additional arguments in JS. The executor is
 * responsible for using Bridge->callNativeModules to invoke any necessary
 * native modules methods.
 */
void JSWasmExecutor::callFunction(
  const std::string &moduleId, const std::string &methodId, const folly::dynamic &arguments) {
  folly::dynamic params = folly::dynamic::array(std::move(moduleId), std::move(methodId), arguments);

  const char *result = ReactNativeWasm::JavascriptAccessor::fbBatchedBridge(
    "callFunctionReturnFlushedQueue", folly::toJson(params).c_str());

  delegate->callNativeModules(*this, folly::parseJson(result), true);
}

/**
 * Executes BatchedBridge.invokeCallbackAndReturnFlushedQueue with the cbID,
 * and optional additional arguments in JS and returns the next queue. The
 * executor is responsible for using Bridge->callNativeModules to invoke any
 * necessary native modules methods.
 */
void JSWasmExecutor::invokeCallback(const double callbackId, const folly::dynamic &arguments) {
  folly::dynamic params = folly::dynamic::array(std::move(callbackId), arguments);

  const char *result = ReactNativeWasm::JavascriptAccessor::fbBatchedBridge(
    "invokeCallbackAndReturnFlushedQueue", folly::toJson(params).c_str());

  delegate->callNativeModules(*this, folly::parseJson(result), true);
}

void JSWasmExecutor::setGlobalVariable(
  std::string propName, std::unique_ptr<const facebook::react::JSBigString> jsonValue) {
  std::cout << "JSWasmExecutor::setGlobalVariable" << std::endl;

  ReactNativeWasm::JavascriptAccessor::setGlobalVariable(propName.c_str(), jsonValue->c_str());
}

/**
 * The description is displayed in the dev menu, if there is one in
 * this build.  There is a default, but if this method returns a
 * non-empty string, it will be used instead.
 */
std::string JSWasmExecutor::getDescription() { return "JSWasmExecutor"; }

std::string JSWasmExecutor::getSyntheticBundlePath(uint32_t bundleId, const std::string &bundlePath) { return "toto"; }
} // namespace ReactNativeWasm
