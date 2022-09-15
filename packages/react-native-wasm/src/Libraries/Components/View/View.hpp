
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
struct View {
  View();

  static std::string getName();

  std::vector<facebook::react::ComponentDescriptorProvider> supplementalComponentDescriptorProviders() noexcept;
  void updateEventEmitter(facebook::react::EventEmitter::Shared const &eventEmitter) noexcept;
  void mountChildComponentView(const View &childComponentView, uint32_t index) noexcept;
  void unmountChildComponentView(const View &childComponentView, uint32_t index) noexcept;
  void updateProps(facebook::react::Props::Shared const &props, facebook::react::Props::Shared const &oldProps) noexcept;
  void updateState(facebook::react::State::Shared const &state, facebook::react::State::Shared const &oldState) noexcept
     ;
  void updateLayoutMetrics(
      facebook::react::LayoutMetrics const &layoutMetrics,
      facebook::react::LayoutMetrics const &oldLayoutMetrics) noexcept;
  void handleCommand(std::string const &commandName, folly::dynamic const &arg) noexcept;
  void finalizeUpdates(RNComponentViewUpdateMask updateMask) noexcept;
  void prepareForRecycle() noexcept;
  facebook::react::Props::Shared props() noexcept;

 private:
  facebook::react::SharedViewProps m_props;
  facebook::react::LayoutMetrics m_layoutMetrics;
};

}
