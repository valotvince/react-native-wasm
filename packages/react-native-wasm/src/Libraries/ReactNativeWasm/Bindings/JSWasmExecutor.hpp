#pragma once

#include <cxxreact/RAMBundleRegistry.h>
#include <cxxreact/JSExecutor.h>
#include <cxxreact/MessageQueueThread.h>
#include <cxxreact/JSBigString.h>
#include <cxxreact/ModuleRegistry.h>
#include <folly/dynamic.h>

namespace ReactNativeWasm {
    class JSWasmExecutorFactory : public facebook::react::JSExecutorFactory {
        public:
            std::unique_ptr<facebook::react::JSExecutor> createJSExecutor(
                std::shared_ptr<facebook::react::ExecutorDelegate> delegate,
                std::shared_ptr<facebook::react::MessageQueueThread> jsQueue);
    };

    class JSWasmExecutor : public facebook::react::JSExecutor {
        public:
            JSWasmExecutor(
                std::shared_ptr<facebook::react::ExecutorDelegate> delegate,
                std::shared_ptr<facebook::react::MessageQueueThread> jsQueue
            ): delegate(delegate), jsQueue(jsQueue) {};

            /**
             * Prepares the JS runtime for React Native by installing global variables.
             * Called once before any JS is evaluated.
             */
            void initializeRuntime() override;
            /**
             * Execute an application script bundle in the JS context.
             */
            void loadBundle(
                std::unique_ptr<const facebook::react::JSBigString> script,
                std::string sourceURL) override;

            /**
             * Add an application "RAM" bundle registry
             */
            void setBundleRegistry(
                std::unique_ptr<facebook::react::RAMBundleRegistry> bundleRegistry) override;

            /**
             * Register a file path for an additional "RAM" bundle
             */
            void registerBundle(
                uint32_t bundleId,
                const std::string &bundlePath) override;

            /**
             * Executes BatchedBridge.callFunctionReturnFlushedQueue with the module ID,
             * method ID and optional additional arguments in JS. The executor is
             * responsible for using Bridge->callNativeModules to invoke any necessary
             * native modules methods.
             */
            void callFunction(
                const std::string &moduleId,
                const std::string &methodId,
                const folly::dynamic &arguments) override;

            /**
             * Executes BatchedBridge.invokeCallbackAndReturnFlushedQueue with the cbID,
             * and optional additional arguments in JS and returns the next queue. The
             * executor is responsible for using Bridge->callNativeModules to invoke any
             * necessary native modules methods.
             */
            void invokeCallback(
                const double callbackId,
                const folly::dynamic &arguments) override;

            void setGlobalVariable(
                std::string propName,
                std::unique_ptr<const facebook::react::JSBigString> jsonValue) override;

            void *getJavaScriptContext() override {
                return nullptr;
            }

            /**
             * Returns whether or not the underlying executor supports debugging via the
             * Chrome remote debugging protocol.
             */
            bool isInspectable() override {
                return false;
            }

            /**
             * The description is displayed in the dev menu, if there is one in
             * this build.  There is a default, but if this method returns a
             * non-empty string, it will be used instead.
             */
            std::string getDescription() override;

            void handleMemoryPressure(__attribute__((unused)) int pressureLevel) override {
            }

            void destroy() override {}
            ~JSWasmExecutor() override {}

            void flush() override {}

            static std::string getSyntheticBundlePath(
                uint32_t bundleId,
                const std::string &bundlePath);

        private:
            std::shared_ptr<facebook::react::ExecutorDelegate> delegate;
            std::shared_ptr<facebook::react::MessageQueueThread> jsQueue;
    };
}
