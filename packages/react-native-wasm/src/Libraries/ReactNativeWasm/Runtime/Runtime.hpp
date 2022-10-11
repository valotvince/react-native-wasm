#pragma once

#include <emscripten/val.h>
#include <iostream>
#include <jsi/jsi.h>
#include <map>

using namespace facebook::jsi;

namespace ReactNativeWasm {
class Runtime : public facebook::jsi::Runtime {
public:
  Runtime() noexcept;
  ~Runtime(){};

  facebook::jsi::Value
  evaluateJavaScript(const std::shared_ptr<const facebook::jsi::Buffer> &buffer, const std::string &sourceURL) override;

  std::shared_ptr<const facebook::jsi::PreparedJavaScript>
  prepareJavaScript(const std::shared_ptr<const facebook::jsi::Buffer> &buffer, std::string sourceURL) override;

  facebook::jsi::Value
  evaluatePreparedJavaScript(const std::shared_ptr<const facebook::jsi::PreparedJavaScript> &js) override;

  bool drainMicrotasks(int maxMicrotasksHint = -1) override;

  facebook::jsi::Object global() override;

  std::string description() override;

  bool isInspectable() override;

  PointerValue *cloneSymbol(const Runtime::PointerValue *pv) override;
  PointerValue *cloneBigInt(const Runtime::PointerValue *pv) override;
  PointerValue *cloneString(const Runtime::PointerValue *pv) override;
  PointerValue *cloneObject(const Runtime::PointerValue *pv) override;
  PointerValue *clonePropNameID(const Runtime::PointerValue *pv) override;
  PropNameID createPropNameIDFromString(const String &str) override;
  PropNameID createPropNameIDFromSymbol(const Symbol &sym) override;
  std::string utf8(const PropNameID &) override;
  std::string utf8(const String &) override;
  bool compare(const PropNameID &, const PropNameID &) override;
  std::string symbolToString(const Symbol &) override;
  String createStringFromAscii(const char *str, size_t length) override;
  String createStringFromUtf8(const uint8_t *utf8, size_t length) override;
  Object createObject() override;
  Object createObject(std::shared_ptr<HostObject> ho) override;
  std::shared_ptr<HostObject> getHostObject(const Object &) override;
  HostFunctionType &getHostFunction(const Function &) override;
  PropNameID createPropNameIDFromAscii(const char *str, size_t length) override;
  PropNameID createPropNameIDFromUtf8(const uint8_t *utf8, size_t length) override;
  Value lockWeakObject(WeakObject &) override;

  Value callAsConstructor(const Function &, const Value *args, size_t count) override;

  Function
  createFunctionFromHostFunction(const PropNameID &name, unsigned int paramCount, HostFunctionType func) override;
  Value call(const Function &, const Value &jsThis, const Value *args, size_t count) override;
  Value getProperty(const Object &, const PropNameID &name) override;
  Value getProperty(const Object &, const String &name) override;
  bool hasProperty(const Object &, const PropNameID &name) override;
  bool hasProperty(const Object &, const String &name) override;
  bool isArray(const Object &) const override;
  bool isArrayBuffer(const Object &) const override;
  bool isFunction(const Object &) const override;
  bool isHostObject(const Object &) const override;
  bool isHostFunction(const Function &) const override;
  Array getPropertyNames(const Object &) override;
  WeakObject createWeakObject(const Object &) override;
  Array createArray(size_t length) override;
  size_t size(const Array &) override;
  size_t size(const ArrayBuffer &) override;
  uint8_t *data(const ArrayBuffer &) override;
  Value getValueAtIndex(const Array &, size_t i) override;
  void setValueAtIndexImpl(Array &, size_t i, const Value &value) override;
  bool strictEquals(const Symbol &a, const Symbol &b) const override;
  bool strictEquals(const BigInt &a, const BigInt &b) const override;
  bool strictEquals(const String &a, const String &b) const override;
  bool strictEquals(const Object &a, const Object &b) const override;
  bool instanceOf(const Object &o, const Function &f) override;

  void setPropertyValue(
    facebook::jsi::Object &, const facebook::jsi::PropNameID &name, const facebook::jsi::Value &value) override;
  void setPropertyValue(
    facebook::jsi::Object &, const facebook::jsi::String &name, const facebook::jsi::Value &value) override;

  void invoke(std::string methodName, std::string jsonArgs);

protected:
  class WasmObjectValue final : public PointerValue {
  public:
    WasmObjectValue(Runtime *runtime, HostFunctionType func) : func(func), isFunction(true){};
    WasmObjectValue(Runtime *runtime, emscripten::val data) : data(data), isFunction(false){};

    HostFunctionType func;
    emscripten::val data;
    bool isFunction;

    void invalidate() override;

  protected:
    friend class Runtime;
  };

  void setPropertyValue(
    WasmObjectValue *decodedObject, const WasmObjectValue *decodedName, const facebook::jsi::Value &value);

  // void setPropertyValue(WasmObjectValue *object, const WasmObjectValue * name, const WasmObjectValue * value);

  // bool test;
  // std::vector<std::string> test1;
  // std::unique_ptr<std::map<std::string, const WasmObjectValue *>> runtimeMethods;
};
} // namespace ReactNativeWasm
