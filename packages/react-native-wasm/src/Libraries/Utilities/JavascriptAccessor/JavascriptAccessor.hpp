#pragma once

#include <stdint.h>
#include <string.h>

extern "C" {
namespace ReactNativeWasm::JavascriptAccessor {
extern void insertScriptTag(const char *scriptUrl);
extern void setGlobalVariable(const char *name, const char *value);
extern const char *fbBatchedBridge(const char *methodName, const char *args);
} // namespace ReactNativeWasm::JavascriptAccessor
}
