#include "View.hpp"
#include "../../ReactNativeWasm/Renderer/Renderer.hpp"

namespace ReactNativeWasm::Components {

View::View() {
  static auto const defaultProps = std::make_shared<facebook::react::ViewProps const>();
  m_props = defaultProps;
}

std::vector<facebook::react::ComponentDescriptorProvider>
View::supplementalComponentDescriptorProviders() noexcept {
  return {};
}

void View::updateEventEmitter(facebook::react::EventEmitter::Shared const &eventEmitter) noexcept {

}

void View::handleCommand(std::string const &commandName, folly::dynamic const &arg) noexcept {

}

void View::mountChildComponentView(const View &childComponentView, uint32_t index) noexcept {
//   m_panel.Children().InsertAt(index, static_cast<const BaseComponentView &>(childComponentView).Element());
}

void View::unmountChildComponentView(const View &childComponentView, uint32_t index) noexcept {
//   m_panel.Children().RemoveAt(index);
}

void View::updateProps(
    facebook::react::Props::Shared const &props,
    facebook::react::Props::Shared const &oldProps) noexcept {
  const auto &oldViewProps = *std::static_pointer_cast<const facebook::react::ViewProps>(m_props);
  const auto &newViewProps = *std::static_pointer_cast<const facebook::react::ViewProps>(props);


  // Do something to update props
  // End update

  m_props = std::static_pointer_cast<facebook::react::ViewProps const>(props);
}

void View::updateState(
    facebook::react::State::Shared const &state,
    facebook::react::State::Shared const &oldState) noexcept {}

void View::updateLayoutMetrics(
    facebook::react::LayoutMetrics const &layoutMetrics,
    facebook::react::LayoutMetrics const &oldLayoutMetrics) noexcept {
}

void View::finalizeUpdates(RNComponentViewUpdateMask updateMask) noexcept {
    // Render

    ReactNativeWasm::Renderer::render(0, 0, 100, 100);
}

void View::prepareForRecycle() noexcept {}
facebook::react::Props::Shared View::props() noexcept {
  assert(false);
  return {};
}
} // namespace Microsoft::ReactNative
