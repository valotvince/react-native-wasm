#pragma once

#include <string.h>
#include <stdint.h>

extern "C" {
  namespace ReactNativeWasm::JavascriptAccessor {
    extern void insertScriptTag(const char *scriptUrl);
    extern void setGlobalVariable(const char *name, const char *value);
    extern void fbBatchedBridge(const char *methodName, const char *args);
  }
}
