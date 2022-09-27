#pragma once

#include <react/renderer/uimanager/UIManagerAnimationDelegate.h>

namespace ReactNativeWasm {
    class UIManagerAnimationDelegate : public facebook::react::UIManagerAnimationDelegate {
        public:
            ~UIManagerAnimationDelegate();

            /*
            * Configure a LayoutAnimation.
            * TODO: need SurfaceId here
            */
            void uiManagerDidConfigureNextLayoutAnimation(
                facebook::jsi::Runtime &runtime,
                facebook::react::RawValue const &config,
                facebook::jsi::Value const &successCallback,
                facebook::jsi::Value const &failureCallback) const;

            /**
             * Set ComponentDescriptor registry.
             *
             * @param componentDescriptorRegistry
             */
            void setComponentDescriptorRegistry(
                const facebook::react::SharedComponentDescriptorRegistry &componentDescriptorRegistry);

            /**
             * Only needed on Android to drive animations.
             */
            bool shouldAnimateFrame() const;

            /**
             * Drop any animations for a given surface.
             */
            void stopSurface(facebook::react::SurfaceId surfaceId);
    };
}
