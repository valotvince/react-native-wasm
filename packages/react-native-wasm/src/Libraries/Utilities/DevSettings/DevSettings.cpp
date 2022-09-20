// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "DevSettings.hpp"
#include <emscripten/bind.h>
#include <cxxreact/CxxNativeModule.h>

using Method = facebook::xplat::module::CxxModule::Method;

namespace ReactNativeWasm {
    std::string DevSettings::getName() {
        return "DevSettings";
    }

    std::vector<Method> DevSettings::getMethods() {
        return {};
    }

    std::map<std::string, folly::dynamic> DevSettings::getConstants() {
        return {};
    }

    void DevSettings::Initialize() noexcept {
    }

    void DevSettings::reload() noexcept {
    //   Reload(m_context.Properties());
    }

    void DevSettings::reloadWithReason(std::string /*reason*/) noexcept {
    }

    void DevSettings::onFastRefresh() noexcept {
    // noop
    }

    void DevSettings::setHotLoadingEnabled(bool isHotLoadingEnabled) noexcept {
    }

    void DevSettings::setIsDebuggingRemotely(bool isDebuggingRemotelyEnabled) noexcept {
    }

    void DevSettings::setProfilingEnabled(bool isProfilingEnabled) noexcept {
    }

    void DevSettings::toggleElementInspector() noexcept {
    }

    void DevSettings::addMenuItem(std::string title) noexcept {
    assert(false);
    }

    void DevSettings::setIsShakeToShowDevMenuEnabled(bool enabled) noexcept {
    assert(false);
    }

    /*static*/ void DevSettings::addListener(std::string eventName) noexcept {
    // noop
    }

    /*static*/ void DevSettings::removeListeners(double count) noexcept {
    // noop
    }

}

// EMSCRIPTEN_BINDINGS(ReactNativeWasmDevSettings) {
//     emscripten::class_<ReactNativeWasm::DevSettings, emscripten::base<facebook::react::NativeModule>>("DevSettings")
//         .smart_ptr<std::shared_ptr<ReactNativeWasm::DevSettings>>("DevSettings")
//         .function("reload", &ReactNativeWasm::DevSettings::reload);
// }
