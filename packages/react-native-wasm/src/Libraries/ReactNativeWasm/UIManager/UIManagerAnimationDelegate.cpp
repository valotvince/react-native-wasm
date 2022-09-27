#include "UIManagerAnimationDelegate.hpp"

namespace ReactNativeWasm {
    UIManagerAnimationDelegate::~UIManagerAnimationDelegate() {}

    /*
    * Configure a LayoutAnimation.
    * TODO: need SurfaceId here
    */
    void UIManagerAnimationDelegate::uiManagerDidConfigureNextLayoutAnimation(
        facebook::jsi::Runtime &runtime,
        facebook::react::RawValue const &config,
        facebook::jsi::Value const &successCallback,
        facebook::jsi::Value const &failureCallback) const {};

    /**
     * Set ComponentDescriptor registry.
     *
     * @param componentDescriptorRegistry
     */
    void UIManagerAnimationDelegate::setComponentDescriptorRegistry(
        const facebook::react::SharedComponentDescriptorRegistry &componentDescriptorRegistry) {}

    /**
     * Only needed on Android to drive animations.
     */
    bool UIManagerAnimationDelegate::shouldAnimateFrame() const { return false; };

    /**
     * Drop any animations for a given surface.
     */
    void UIManagerAnimationDelegate::stopSurface(facebook::react::SurfaceId surfaceId) {}
}
