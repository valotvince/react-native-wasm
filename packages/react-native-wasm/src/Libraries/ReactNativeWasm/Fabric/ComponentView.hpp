#pragma once

#include <react/renderer/components/view/ViewProps.h>
#include <react/renderer/mounting/ShadowView.h>
#include <vector>

namespace ReactNativeWasm {
class ComponentView {
public:
  using Shared = std::shared_ptr<ComponentView>;

  ComponentView() { props = std::make_shared<facebook::react::ViewProps const>(); };

  void update(const facebook::react::ShadowView &);
  void addChild(const facebook::react::Tag, std::shared_ptr<ComponentView>);
  void removeChild(const facebook::react::Tag);

  facebook::react::Props::Shared props;
  facebook::react::State::Shared state;
  facebook::react::LayoutMetrics layoutMetrics;

  std::string name;

  // TODO Rendering: Use a greater to render upper items first to prevent them to render over nested views
  std::map<const facebook::react::Tag, std::shared_ptr<ComponentView>, std::greater<facebook::react::Tag>> children;
};
} // namespace ReactNativeWasm
