#include "Runtime.hpp"
#include "../../Utilities/JavascriptAccessor/JavascriptAccessor.hpp"
#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <folly/dynamic.h>
#include <folly/json.h>
#include <iostream>
#include <jsi/JSIDynamic.h>

extern "C" {
namespace ReactNativeWasm::JavascriptAccessor {
extern void setGlobalVariableFunction(const char *name);
extern void setGlobalVariableObject(const char *, facebook::jsi::HostObject &);
} // namespace ReactNativeWasm::JavascriptAccessor
}

namespace ReactNativeWasm {

auto runtimeMethods = std::map<std::string, WasmFunctionCallback *>();

void Runtime::WasmObjectValue::invalidate() {}

Runtime::Runtime() noexcept {};

facebook::jsi::Value
Runtime::evaluateJavaScript(const std::shared_ptr<const facebook::jsi::Buffer> &buffer, const std::string &sourceURL) {
  std::cout << "Runtime::evaluateJavaScript" << std::endl;
  return 0;
};

std::shared_ptr<const facebook::jsi::PreparedJavaScript>
Runtime::prepareJavaScript(const std::shared_ptr<const facebook::jsi::Buffer> &buffer, std::string sourceURL) {
  std::cout << "Runtime::prepareJavaScript" << std::endl;
  return nullptr;
};

facebook::jsi::Value
Runtime::evaluatePreparedJavaScript(const std::shared_ptr<const facebook::jsi::PreparedJavaScript> &js) {
  std::cout << "Runtime::evaluatePreparedJavaScript" << std::endl;
  // use emscripten_run_script("alert('hi')")

  return 0;
};

bool Runtime::drainMicrotasks(int maxMicrotasksHint) {
  std::cout << "Runtime::drainMicrotasks" << std::endl;
  return false;
};

facebook::jsi::Object Runtime::global() {
  auto window = emscripten::val::global("window");

  return make<Object>(new WasmObjectValue(this, std::move(window)));
  ;
};

std::string Runtime::description() { return "WASM Runtime"; };

bool Runtime::isInspectable() { return false; };

Runtime::PointerValue *Runtime::cloneSymbol(const Runtime::PointerValue *pv) {
  std::cout << "Runtime::cloneSymbol" << std::endl;
  throw std::logic_error("Not implemented");
};
Runtime::PointerValue *Runtime::cloneBigInt(const Runtime::PointerValue *pv) {
  std::cout << "Runtime::cloneBigInt" << std::endl;
  throw std::logic_error("Not implemented");
};
Runtime::PointerValue *Runtime::cloneString(const Runtime::PointerValue *pv) {
  std::cout << "Runtime::cloneString" << std::endl;
  throw std::logic_error("Not implemented");
};
Runtime::PointerValue *Runtime::cloneObject(const Runtime::PointerValue *pv) {
  std::cout << "Runtime::cloneObject" << std::endl;

  if (!pv) {
    return nullptr;
  }

  auto *object = static_cast<const WasmObjectValue *>(pv);

  return new WasmObjectValue(this, object->data);
};
Runtime::PointerValue *Runtime::clonePropNameID(const Runtime::PointerValue *pv) {
  std::cout << "Runtime::clonePropNameID" << std::endl;
  throw std::logic_error("Not implemented");
};
PropNameID Runtime::createPropNameIDFromString(const String &str) {
  std::cout << "Runtime::createPropNameIDFromString" << std::endl;
  throw std::logic_error("Not implemented");
};
PropNameID Runtime::createPropNameIDFromSymbol(const Symbol &sym) {
  std::cout << "Runtime::createPropNameIDFromSymbol" << std::endl;
  throw std::logic_error("Not implemented");
};
std::string Runtime::utf8(const PropNameID &object) {
  std::cout << "Runtime::utf8 1" << std::endl;

  auto decodedObject = static_cast<const WasmObjectValue *>(getPointerValue(object));

  if (!decodedObject->data.isString()) {
    throw std::logic_error("Object is not a string");
  }

  return decodedObject->data.as<std::string>();
};
std::string Runtime::utf8(const String &object) {
  std::cout << "Runtime::utf8 2" << std::endl;

  auto decodedObject = static_cast<const WasmObjectValue *>(getPointerValue(object));

  if (!decodedObject->data.isString()) {
    throw std::logic_error("Object is not a string");
  }

  return decodedObject->data.as<std::string>();
};
bool Runtime::compare(const PropNameID &, const PropNameID &) {
  std::cout << "Runtime::compare" << std::endl;

  throw std::logic_error("Not implemented");
};
std::string Runtime::symbolToString(const Symbol &) {
  std::cout << "Runtime::symbolToString" << std::endl;
  throw std::logic_error("Not implemented");
};
String Runtime::createStringFromAscii(const char *str, size_t length) {
  std::cout << "Runtime::createStringFromAscii" << std::endl;

  auto val = emscripten::val(str);

  return make<String>(new WasmObjectValue(this, std::move(val)));
};
String Runtime::createStringFromUtf8(const uint8_t *utf8, size_t length) {
  std::cout << "Runtime::createStringFromUtf8" << std::endl;

  auto val = emscripten::val::u8string(reinterpret_cast<const char *>(utf8));

  return make<String>(new WasmObjectValue(this, std::move(val)));
};
Object Runtime::createObject() {
  std::cout << "Runtime::createObject" << std::endl;

  auto val = emscripten::val::object();

  return make<Object>(new WasmObjectValue(this, std::move(val)));
};
Object Runtime::createObject(std::shared_ptr<HostObject> ho) {
  std::cout << "Runtime::createObject 2" << std::endl;

  // auto Proxy = emscripten::val::global("Proxy");
  auto obj = emscripten::val::object();

  // obj.set("get", [](emscripten::val name) {
  //   std::cout << name.as<std::string>() << std::endl;
  // });

  // auto proxy = Proxy.new_(obj);

  return make<Object>(new WasmObjectValue(this, std::move(obj)));
};
std::shared_ptr<HostObject> Runtime::getHostObject(const Object &) {
  std::cout << "Runtime::getHostObject" << std::endl;
  throw std::logic_error("Not implemented");
};
HostFunctionType &Runtime::getHostFunction(const Function &) {
  std::cout << "Runtime::getHostFunction" << std::endl;
  throw std::logic_error("Not implemented");
};
PropNameID Runtime::createPropNameIDFromAscii(const char *str, size_t length) {
  std::cout << "Runtime::createPropNameIDFromAscii" << std::endl;

  auto val = emscripten::val::u8string(str);

  return make<PropNameID>(new WasmObjectValue(this, std::move(val)));
};
PropNameID Runtime::createPropNameIDFromUtf8(const uint8_t *utf8, size_t length) {
  std::cout << "Runtime::createPropNameIDFromUtf8" << std::endl;

  auto val = emscripten::val::u8string(reinterpret_cast<const char *>(utf8));

  return make<PropNameID>(new WasmObjectValue(this, std::move(val)));
};
Value Runtime::lockWeakObject(WeakObject &) {
  std::cout << "Runtime::lockWeakObject" << std::endl;
  throw std::logic_error("Not implemented");
};

Value Runtime::callAsConstructor(const Function &, const Value *args, size_t count) {
  std::cout << "Runtime::callAsConstructor" << std::endl;
  throw std::logic_error("Not implemented");
};

Function
Runtime::createFunctionFromHostFunction(const PropNameID &name, unsigned int paramCount, HostFunctionType func) {
  std::cout << "Runtime::createFunctionFromHostFunction" << std::endl;

  auto val = emscripten::val::object();
  val.set("name", utf8(name));
  val.set("paramCount", paramCount);
  val.set("funcAddr", reinterpret_cast<uintptr_t>(&func));

  auto object = make<Object>(new WasmObjectValue(this, std::move(val)));

  return object.getFunction(*this);
};
Value Runtime::call(const Function &, const Value &jsThis, const Value *args, size_t count) {
  std::cout << "Runtime::call" << std::endl;
  throw std::logic_error("Not implemented");
};
Value Runtime::getProperty(const Object &object, const PropNameID &name) {
  auto decodedName = utf8(name);
  auto decodedObject = static_cast<const WasmObjectValue *>(getPointerValue(object));

  std::cout << "Runtime::getProperty 1 " << decodedName << std::endl;

  auto val = decodedObject->data;

  std::cout << val[decodedName].typeOf().as<std::string>() << std::endl;

  auto propertyValue = val[decodedName];

  return Value(make<Object>(new WasmObjectValue(this, std::move(propertyValue))));
};
Value Runtime::getProperty(const Object &object, const String &name) {
  auto decodedName = utf8(name);
  auto decodedObject = static_cast<const WasmObjectValue *>(getPointerValue(object));

  std::cout << "Runtime::getProperty 2 " << decodedName << std::endl;

  auto val = decodedObject->data;

  std::cout << val[decodedName].typeOf().as<std::string>() << std::endl;

  auto propertyValue = val[decodedName];

  return Value(make<Object>(new WasmObjectValue(this, std::move(propertyValue))));
};
bool Runtime::hasProperty(const Object &object, const PropNameID &name) {
  std::cout << "Runtime::hasProperty 1" << std::endl;

  auto decodedName = utf8(name);
  auto decodedObject = static_cast<const WasmObjectValue *>(getPointerValue(object));

  return decodedObject->data.hasOwnProperty(decodedName.c_str());
};
bool Runtime::hasProperty(const Object &object, const String &name) {
  std::cout << "Runtime::hasProperty 2" << std::endl;

  auto decodedName = utf8(name);
  auto decodedObject = static_cast<const WasmObjectValue *>(getPointerValue(object));

  return decodedObject->data.hasOwnProperty(decodedName.c_str());
};

bool Runtime::isArray(const Object &object) const {
  std::cout << "Runtime::isArray" << std::endl;

  auto decodedObject = static_cast<const WasmObjectValue *>(getPointerValue(object));

  return decodedObject->data.isArray();
};

bool Runtime::isArrayBuffer(const Object &object) const {
  std::cout << "Runtime::isArrayBuffer" << std::endl;

  auto decodedObject = static_cast<const WasmObjectValue *>(getPointerValue(object));

  return decodedObject->data.instanceof (emscripten::val::global("ArrayBuffer"));
};

bool Runtime::isFunction(const Object &object) const {
  std::cout << "Runtime::isFunction" << std::endl;

  auto decodedObject = static_cast<const WasmObjectValue *>(getPointerValue(object));

  return decodedObject->data.hasOwnProperty("funcAddr");
};
bool Runtime::isHostObject(const Object &) const {
  std::cout << "Runtime::isHostObject" << std::endl;
  throw std::logic_error("Not implemented");
};
bool Runtime::isHostFunction(const Function &) const {
  std::cout << "Runtime::isHostFunction" << std::endl;
  throw std::logic_error("Not implemented");
};
Array Runtime::getPropertyNames(const Object &) {
  std::cout << "Runtime::getPropertyNames" << std::endl;
  throw std::logic_error("Not implemented");
};
WeakObject Runtime::createWeakObject(const Object &) {
  std::cout << "Runtime::createWeakObject" << std::endl;

  throw std::logic_error("Not implemWented");
};
Array Runtime::createArray(size_t length) {
  std::cout << "Runtime::createArray" << std::endl;

  auto array = emscripten::val::array();

  auto object = make<Object>(new WasmObjectValue(this, array));

  return object.getArray(*this);
};
size_t Runtime::size(const Array &object) {
  std::cout << "Runtime::size 1" << std::endl;

  auto decodedObject = static_cast<const WasmObjectValue *>(getPointerValue(object));

  return decodedObject->data["length"].as<size_t>();
};
size_t Runtime::size(const ArrayBuffer &object) {
  std::cout << "Runtime::size 2" << std::endl;

  auto decodedObject = static_cast<const WasmObjectValue *>(getPointerValue(object));

  return decodedObject->data["length"].as<size_t>();
};
uint8_t *Runtime::data(const ArrayBuffer &) {
  std::cout << "Runtime::data" << std::endl;
  throw std::logic_error("Not implemented");
};
Value Runtime::getValueAtIndex(const Array &object, size_t i) {
  std::cout << "Runtime::getValueAtIndex" << std::endl;

  auto decodedObject = static_cast<const WasmObjectValue *>(getPointerValue(object));
  throw std::logic_error("Not implemented");
};
void Runtime::setValueAtIndexImpl(Array &object, size_t i, const Value &value) {
  std::cout << "Runtime::setValueAtIndexImpl" << std::endl;

  auto decodedObject = static_cast<const WasmObjectValue *>(getPointerValue(object));
  auto decodedValue = static_cast<const WasmObjectValue *>(getPointerValue(value));

  // decodedObject->data.set(i, decodedValue->data);
};
bool Runtime::strictEquals(const Symbol &a, const Symbol &b) const {
  std::cout << "Runtime::strictEquals 1" << std::endl;
  throw std::logic_error("Not implemented");
};
bool Runtime::strictEquals(const BigInt &a, const BigInt &b) const {
  std::cout << "Runtime::strictEquals 2" << std::endl;
  throw std::logic_error("Not implemented");
};
bool Runtime::strictEquals(const String &a, const String &b) const {
  std::cout << "Runtime::strictEquals 3" << std::endl;
  throw std::logic_error("Not implemented");
};
bool Runtime::strictEquals(const Object &a, const Object &b) const {
  std::cout << "Runtime::strictEquals 4" << std::endl;
  throw std::logic_error("Not implemented");
};
bool Runtime::instanceOf(const Object &o, const Function &f) {
  std::cout << "Runtime::instanceOf" << std::endl;
  throw std::logic_error("Not implemented");
};

void Runtime::setPropertyValue(
  facebook::jsi::Object &, const facebook::jsi::PropNameID &name, const facebook::jsi::Value &value) {
  std::cout << "Runtime::setPropertyValue 1" << std::endl;
  throw std::logic_error("Not implemented");
};
void Runtime::setPropertyValue(
  facebook::jsi::Object &object, const facebook::jsi::String &name, const facebook::jsi::Value &value) {
  auto decodedObject = static_cast<WasmObjectValue *>(getPointerValue(object));
  auto decodedName = static_cast<const WasmObjectValue *>(getPointerValue(name));

  std::cout << "Runtime::setPropertyValue" << std::endl;

  auto humanDecodedName = decodedName->data.as<std::string>();

  auto wasmValue = static_cast<const WasmObjectValue *>(getPointerValue(value));

  decodedObject->data.set(decodedName->data, wasmValue->data);

  // if (value.isString()) {
  //   // ReactNativeWasm::JavascriptAccessor::setGlobalVariable(decodedName, wasmValue->str);
  // } else if (value.isObject()) {
  //   auto decodedValueObject = static_cast<const WasmObjectValue *>(getPointerValue(value));

  //   if (isFunction(*decodedValueObject)) {
  //     std::cout << "Runtime::setPropertyValue function global:" << decodedObject->global << " name:" <<
  //     humanDecodedName
  //               << std::endl;

  //     auto wasmCallback = static_cast<WasmFunctionCallback *>(decodedValueObject->data);

  //     ReactNativeWasm::runtimeMethods.insert(std::make_pair(humanDecodedName, wasmCallback));

  //     // auto runner = [this, object, decodedValueObject](const Value* args, size_t count) {
  //     //   auto function = static_cast<HostFunctionType *>(decodedValueObject->data);
  //     //   auto functionValue = *function;

  //     //   functionValue(*this, object, args, count);
  //     // };

  //     // auto runner = []() { std::cout << "Running C++s function from Javascript" << std::endl; };

  //     // emscripten::val method = emscripten::val::module_property("getDynCaller")(std::string("v"), int(runner));

  //     // ReactNativeWasm::JavascriptAccessor::setGlobalVariableFunction(decodedName);

  //     // auto functor_adapter = emscripten::val(runner)["opcall"].call<emscripten::val>("bind",
  //     // emscripten::val(runner));

  //    decodedObject->data.set(*decodedName->data, *wasmValue->data);
  //   } else {
  //     auto hostObject = static_cast<facebook::jsi::HostObject *>(decodedValueObject->data);

  //     std::cout << "Runtime::setPropertyValue object global:" << decodedObject->global << " name:" << decodedName
  //               << std::endl;

  //     decodedObject->data.set(decodedName->data, *hostObject);
  //   }
  // } else {
  //   throw std::logic_error("Not implemented");
  // }
  // throw std::logic_error("Not implemented");
};

void WasmFunctionCallback::invoke(Value *args) {
  std::cout << "Calling a C++ function from JS" << std::endl;

  auto runtimePtr = static_cast<ReactNativeWasm::Runtime *>(runtime);
  Runtime &runtimeRef = *runtimePtr;

  auto argsCount = args->getObject(runtimeRef).getArray(runtimeRef).size(runtimeRef);
  auto thisVal = Value();

  try {
    this->callback(runtimeRef, thisVal, args, 1);
  } catch (std::exception error) {
    std::cerr << error.what() << std::endl;

    throw error;
  }
}

} // namespace ReactNativeWasm

void runtimeInvokeFunction(std::string methodName, std::string jsonArgs) {
  auto args = folly::parseJson(jsonArgs);

  auto pair = ReactNativeWasm::runtimeMethods.find(methodName);

  if (pair != ReactNativeWasm::runtimeMethods.end()) {
    auto runtime = static_cast<ReactNativeWasm::Runtime *>(pair->second->runtime);
    Value args = facebook::jsi::valueFromDynamic(*runtime, std::move(folly::parseJson(jsonArgs)));
    Value *argsPtr = &args;

    pair->second->invoke(argsPtr);

    return;
  }

  throw std::invalid_argument("No " + methodName + " method registered");

  // auto wasmCallback = static_cast<ReactNativeWasm::WasmFunctionCallback *>(callback);
  // callback->invoke();
}

EMSCRIPTEN_BINDINGS(ReactWasmRuntime) {
  emscripten::class_<facebook::jsi::HostObject>("jsiHostObject")
    .smart_ptr<std::shared_ptr<facebook::jsi::HostObject>>("jsiHostObject")
    .function("get", &facebook::jsi::HostObject::get)
    .function("set", &facebook::jsi::HostObject::set);

  emscripten::function("runtimeInvokeFunction", &runtimeInvokeFunction, emscripten::allow_raw_pointers());
}
