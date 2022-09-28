
#pragma once

#include <react/renderer/components/view/ViewEventEmitter.h>
#include <react/renderer/components/view/ViewProps.h>
#include <react/renderer/core/State.h>
#include <react/renderer/core/LayoutMetrics.h>
#include <react/renderer/core/Props.h>
#include <react/renderer/componentregistry/ComponentDescriptorProvider.h>

enum class RNComponentViewUpdateMask : std::uint_fast8_t {
  None = 0,
  Props = 1 << 0,
  EventEmitter = 1 << 1,
  State = 1 << 2,
  LayoutMetrics = 1 << 3,

  All = Props | EventEmitter | State | LayoutMetrics
};

// DEFINE_ENUM_FLAG_OPERATORS(RNComponentViewUpdateMask);

namespace ReactNativeWasm::Components {
    struct Component {
        static std::string getName();

        virtual std::vector<facebook::react::ComponentDescriptorProvider> supplementalComponentDescriptorProviders() noexcept = 0;
        virtual void updateEventEmitter(facebook::react::EventEmitter::Shared const &eventEmitter) noexcept = 0;
        virtual void mountChildComponentView(const Component &childComponentView, uint32_t index) noexcept = 0;
        virtual void unmountChildComponentView(const Component &childComponentView, uint32_t index) noexcept = 0;
        virtual void updateProps(facebook::react::Props::Shared const &props, facebook::react::Props::Shared const &oldProps) noexcept = 0;
        virtual void updateState(facebook::react::State::Shared const &state, facebook::react::State::Shared const &oldState) noexcept = 0;
        virtual void updateLayoutMetrics(
            facebook::react::LayoutMetrics const &layoutMetrics,
            facebook::react::LayoutMetrics const &oldLayoutMetrics) noexcept = 0;
        virtual void handleCommand(std::string const &commandName, folly::dynamic const &arg) noexcept = 0;
        virtual void finalizeUpdates(RNComponentViewUpdateMask updateMask) noexcept = 0;
        virtual void prepareForRecycle() noexcept = 0;
        virtual facebook::react::Props::Shared props() noexcept = 0;
    };
}