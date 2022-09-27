
#pragma once

#include <react/renderer/components/view/ViewEventEmitter.h>
#include <react/renderer/components/view/ViewProps.h>
#include <react/renderer/core/State.h>
#include <react/renderer/core/LayoutMetrics.h>
#include <react/renderer/core/Props.h>
#include <react/renderer/componentregistry/ComponentDescriptorProvider.h>

#include "../Component.hpp"


// DEFINE_ENUM_FLAG_OPERATORS(RNComponentViewUpdateMask);

namespace ReactNativeWasm::Components {
struct View : Component {
  View();

  static std::string getName();

  std::vector<facebook::react::ComponentDescriptorProvider> supplementalComponentDescriptorProviders() noexcept override;
  void updateEventEmitter(facebook::react::EventEmitter::Shared const &eventEmitter) noexcept override;
  void mountChildComponentView(const Component &childComponentView, uint32_t index) noexcept override;
  void unmountChildComponentView(const Component &childComponentView, uint32_t index) noexcept override;
  void updateProps(facebook::react::Props::Shared const &props, facebook::react::Props::Shared const &oldProps) noexcept override;
  void updateState(facebook::react::State::Shared const &state, facebook::react::State::Shared const &oldState) noexcept override
     ;
  void updateLayoutMetrics(
      facebook::react::LayoutMetrics const &layoutMetrics,
      facebook::react::LayoutMetrics const &oldLayoutMetrics) noexcept override;
  void handleCommand(std::string const &commandName, folly::dynamic const &arg) noexcept override;
  void finalizeUpdates(RNComponentViewUpdateMask updateMask) noexcept override;
  void prepareForRecycle() noexcept override;
  facebook::react::Props::Shared props() noexcept override;

 private:
  facebook::react::SharedViewProps m_props;
  facebook::react::LayoutMetrics m_layoutMetrics;
};

}
