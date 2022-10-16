#include "ComponentView.hpp"

namespace ReactNativeWasm {
void ComponentView::update(const facebook::react::ShadowView &view) {
  name = std::string(view.componentName);

  props = view.props;
  state = view.state;
  layoutMetrics = facebook::react::LayoutMetrics{view.layoutMetrics};
}

void ComponentView::addChild(const facebook::react::Tag tagId, std::shared_ptr<ComponentView> child) {
  children.insert({tagId, child});
  // Do something
}

void ComponentView::removeChild(const facebook::react::Tag tagId) { children.erase({tagId}); }
} // namespace ReactNativeWasm
