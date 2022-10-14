#include "WebGLRenderer.hpp"

#include <GLES2/gl2.h>
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

namespace ReactNativeWasm {
  WebGLRenderer::WebGLRenderer() {}

  void WebGLRenderer::flush() {}

  void WebGLRenderer::render(const facebook::react::ShadowView &view) {
    std::string componentName(view.componentName);

    if (componentName == "Paragraph") {
      // renderText(view);
    } else {
      // renderView(view);
    }
  }
}
