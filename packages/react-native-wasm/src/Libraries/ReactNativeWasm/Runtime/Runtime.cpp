#include "Runtime.hpp"
#include "../../Utilities/JavascriptAccessor/JavascriptAccessor.hpp"
#include <emscripten/bind.h>
#include <emscripten/emscripten.h>
#include <emscripten/val.h>
#include <folly/dynamic.h>
#include <folly/json.h>
#include <iostream>
#include <jsi/JSIDynamic.h>

extern "C" {
namespace ReactNativeWasm::JavascriptAccessor {
extern void setGlobalVariableFunction(const char *);
extern void setGlobalVariableObject(const char *);
} // namespace ReactNativeWasm::JavascriptAccessor
}

namespace ReactNativeWasm {

auto runtimeMethods = std::map<std::string, const void *>();

void Runtime::WasmObjectValue::invalidate() {}

Runtime::Runtime() noexcept {
  emscripten::val::global("window").set("WasmRuntime", this);
  // runtimeMethods = std::make_unique<std::map<std::string, const WasmObjectValue *>>();
};

facebook::jsi::Value
Runtime::evaluateJavaScript(const std::shared_ptr<const facebook::jsi::Buffer> &buffer, const std::string &sourceURL) {
  std::cout << "Runtime::evaluateJavaScript temp4" << std::endl;
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
  auto object = static_cast<const WasmObjectValue *>(pv);

  return new WasmObjectValue(this, emscripten::val(object->data));
};
Runtime::PointerValue *Runtime::cloneObject(const Runtime::PointerValue *pv) {
  if (!pv) {
    return nullptr;
  }

  auto object = static_cast<const WasmObjectValue *>(pv);

  if (object->isFunction) {
    return new WasmObjectValue(this, object->func);
  }

  if (object->isHostObjectProxy) {
    return new WasmObjectValue(this, object->hostObjectProxy);
  }

  return new WasmObjectValue(this, emscripten::val(object->data));
};
Runtime::PointerValue *Runtime::clonePropNameID(const Runtime::PointerValue *pv) {
  std::cout << "Runtime::clonePropNameID" << std::endl;
  throw std::logic_error("Not implemented");
};
PropNameID Runtime::createPropNameIDFromString(const String &str) {
  std::cout << "Runtime::createPropNameIDFromString" << std::endl;

  auto decodedObject = static_cast<const WasmObjectValue *>(getPointerValue(str));

  auto val = emscripten::val(decodedObject->data);

  return make<PropNameID>(new WasmObjectValue(this, std::move(val)));
};
PropNameID Runtime::createPropNameIDFromSymbol(const Symbol &sym) {
  std::cout << "Runtime::createPropNameIDFromSymbol" << std::endl;
  throw std::logic_error("Not implemented");
};
std::string Runtime::utf8(const PropNameID &object) {
  auto decodedObject = static_cast<const WasmObjectValue *>(getPointerValue(object));

  return decodedObject->data.as<std::string>();
};
std::string Runtime::utf8(const String &object) {
  auto decodedObject = static_cast<const WasmObjectValue *>(getPointerValue(object));

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
  auto val = emscripten::val(str);

  return make<String>(new WasmObjectValue(this, std::move(val)));
};
String Runtime::createStringFromUtf8(const uint8_t *utf8, size_t length) {
  auto val = emscripten::val::u8string(reinterpret_cast<const char *>(utf8));

  return make<String>(new WasmObjectValue(this, std::move(val)));
};
Object Runtime::createObject() {
  auto val = emscripten::val::object();

  return make<Object>(new WasmObjectValue(this, std::move(val)));
};
Object Runtime::createObject(std::shared_ptr<HostObject> ho) {
  std::cout << "Runtime::createObject 2" << std::endl;

  // obj.set("get", [](emscripten::val name) {
  //   std::cout << name.as<std::string>() << std::endl;
  // });

  // auto obj = emscripten::val::object();
  // auto properties = ho->getPropertyNames(*this);

  // for (auto it = properties.begin(); it < properties.end(); ++it) {
  //   auto propertyName = it->utf8(*this);

  //   obj.set(propertyName, emscripten::val::null());

  //   auto property = ho->get(*this, *it);
  //   auto wasmValue = static_cast<const WasmObjectValue *>(getPointerValue(property));

  //   std::cout << "property:" << propertyName << " " << wasmValue->isFunction << std::endl;
  // }

  auto proxy = std::make_shared<ReactNativeWasm::Runtime::HostObjectProxy>(*this, ho);

  return make<Object>(new WasmObjectValue(this, std::move(proxy)));
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
  auto val = emscripten::val(str);

  return make<PropNameID>(new WasmObjectValue(this, std::move(val)));
};
PropNameID Runtime::createPropNameIDFromUtf8(const uint8_t *utf8, size_t length) {
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
  std::cout << "Runtime::createFunctionFromHostFunction name:" << name.utf8(*this) << std::endl;

  auto object = make<Object>(new WasmObjectValue(this, func));

  return object.getFunction(*this);
};

emscripten::val Runtime::toEmscriptenVal(const Value &value) {
  if (value.isNull()) {
    return emscripten::val::null();
  } else if (value.isNumber()) {
    return emscripten::val(value.getNumber());
  } else if (value.isBool()) {
    return emscripten::val(value.getBool());
  } else if (value.isUndefined()) {
    return emscripten::val::undefined();
  } else {
    auto wasmValue = static_cast<const WasmObjectValue *>(getPointerValue(value));

    if (wasmValue->isFunction) {
      std::cerr << "Function to Emscripten::val This is not supported for now" << std::endl;
      // std::cout << "Function " << humanDecodedName << std::endl;
      // runtimeMethods.insert({humanDecodedName, wasmValue});
      // ReactNativeWasm::JavascriptAccessor::setGlobalVariableFunction(humanDecodedName.c_str());

      return emscripten::val::null();
    } else if (wasmValue->isHostObjectProxy) {
      return emscripten::val(wasmValue->hostObjectProxy);
    }

    return wasmValue->data;
  }
}

Value Runtime::call(const Function &function, const Value &jsThis, const Value *args, size_t count) {
  std::cout << "Runtime::call" << std::endl;

  auto decodedFunction = static_cast<const WasmObjectValue *>(getPointerValue(function));
  auto decodedArgs = static_cast<const WasmObjectValue *>(getPointerValue(*args));

  std::cout << decodedArgs->data.typeOf().as<std::string>() << std::endl;

  std::vector<emscripten::val> jsArgs;

  for (auto i = 0; i < count; i++) {
    auto value = toEmscriptenVal(args[i]);

    jsArgs.push_back(value);
  }

  emscripten::val returnValue;

  // Uglier could not be possible, but i'll refactor it once I understand cpp pack/unpack
  // TODO: Use argument unpack
  switch (count) {
  case 1:
    returnValue = decodedFunction->data(jsArgs[0]);
    break;
  case 2:
    returnValue = decodedFunction->data(jsArgs[0], jsArgs[1]);
    break;
  case 3:
    returnValue = decodedFunction->data(jsArgs[0], jsArgs[1], jsArgs[2]);
    break;
  case 4:
    returnValue = decodedFunction->data(jsArgs[0], jsArgs[1], jsArgs[2], jsArgs[3]);
    break;
  }

  return Value(make<Object>(new WasmObjectValue(this, std::move(returnValue))));
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
  auto decodedName = utf8(name);
  auto decodedObject = static_cast<const WasmObjectValue *>(getPointerValue(object));

  return decodedObject->data.hasOwnProperty(decodedName.c_str());
};
bool Runtime::hasProperty(const Object &object, const String &name) {
  auto decodedName = utf8(name);
  auto decodedObject = static_cast<const WasmObjectValue *>(getPointerValue(object));

  return decodedObject->data.hasOwnProperty(decodedName.c_str());
};

bool Runtime::isArray(const Object &object) const {
  auto decodedObject = static_cast<const WasmObjectValue *>(getPointerValue(object));

  return decodedObject->data.isArray();
};

bool Runtime::isArrayBuffer(const Object &object) const {
  auto decodedObject = static_cast<const WasmObjectValue *>(getPointerValue(object));

  return decodedObject->data.instanceof (emscripten::val::global("ArrayBuffer"));
};

bool Runtime::isFunction(const Object &object) const {
  auto decodedObject = static_cast<const WasmObjectValue *>(getPointerValue(object));

  if (decodedObject->isFunction) {
    return true;
  }

  return decodedObject->data.typeOf().as<std::string>() == "function";
};
bool Runtime::isHostObject(const Object &object) const {
  std::cout << "Runtime::isHostObject" << std::endl;

  auto decodedObject = static_cast<const WasmObjectValue *>(getPointerValue(object));

  return decodedObject->isHostObjectProxy;
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
  auto decodedObject = static_cast<const WasmObjectValue *>(getPointerValue(object));

  return decodedObject->data["length"].as<size_t>();
};
size_t Runtime::size(const ArrayBuffer &object) {
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

  auto val = decodedObject->data[i];

  return Value(make<Object>(new WasmObjectValue(this, std::move(val))));
};
void Runtime::setValueAtIndexImpl(Array &object, size_t i, const Value &value) {
  auto decodedObject = static_cast<WasmObjectValue *>(getPointerValue(object));
  // auto decodedValue = static_cast<const WasmObjectValue *>(getPointerValue(value));

  decodedObject->data.set(i, toEmscriptenVal(value));
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
};
bool Runtime::instanceOf(const Object &o, const Function &f) {
  std::cout << "Runtime::instanceOf" << std::endl;
  throw std::logic_error("Not implemented");
};

void Runtime::setPropertyValue(
  WasmObjectValue *decodedObject, const WasmObjectValue *decodedName, const facebook::jsi::Value &value) {
  auto humanDecodedName = decodedName->data.as<std::string>();

  std::cout << "Runtime::setPropertyValue " << humanDecodedName << value.isNull() << std::endl;

  if (value.isNull()) {
    decodedObject->data.set(decodedName->data, emscripten::val::null());
  } else if (value.isNumber()) {
    decodedObject->data.set(decodedName->data, value.getNumber());
  } else if (value.isBool()) {
    decodedObject->data.set(decodedName->data, value.getBool());
  } else if (value.isUndefined()) {
    decodedObject->data.set(decodedName->data, emscripten::val::undefined());
    // } else if (value.isObject()) {
    // decodedObject->data.set(decodedName->data, value.getBigInt());
  } else {
    auto wasmValue = static_cast<const WasmObjectValue *>(getPointerValue(value));

    if (wasmValue->isFunction) {
      std::cout << "Function " << humanDecodedName << std::endl;
      runtimeMethods.insert({humanDecodedName, wasmValue});
      ReactNativeWasm::JavascriptAccessor::setGlobalVariableFunction(humanDecodedName.c_str());
    } else if (wasmValue->isHostObjectProxy) {
      std::cout << "Host object " << humanDecodedName << std::endl;

      decodedObject->data.set("native__" + humanDecodedName, wasmValue->hostObjectProxy);

      ReactNativeWasm::JavascriptAccessor::setGlobalVariableObject(humanDecodedName.c_str());

    } else {
      std::cout << "Value " << humanDecodedName << std::endl;
      decodedObject->data.set(decodedName->data, wasmValue->data);
    }
  }
}

void Runtime::setPropertyValue(
  facebook::jsi::Object &object, const facebook::jsi::PropNameID &name, const facebook::jsi::Value &value) {
  auto decodedObject = static_cast<WasmObjectValue *>(getPointerValue(object));
  auto decodedName = static_cast<const WasmObjectValue *>(getPointerValue(name));

  setPropertyValue(decodedObject, decodedName, value);
};
void Runtime::setPropertyValue(
  facebook::jsi::Object &object, const facebook::jsi::String &name, const facebook::jsi::Value &value) {
  auto decodedObject = static_cast<WasmObjectValue *>(getPointerValue(object));
  auto decodedName = static_cast<const WasmObjectValue *>(getPointerValue(name));

  setPropertyValue(decodedObject, decodedName, value);
}

emscripten::val Runtime::invokeHostFunction(HostFunctionType func, emscripten::val jsonArgs) {
  auto size = jsonArgs["length"].as<size_t>();

  Value stackArgs[size];

  for (auto i = 0; i < size; i++) {
    auto arg = jsonArgs[i];

    if (arg.isString()) {
      stackArgs[i] = Value(make<String>(new WasmObjectValue(this, std::move(arg))));
    } else if (arg.isArray()) {
      stackArgs[i] = Value(make<Object>(new WasmObjectValue(this, std::move(arg))).getArray(*this));
    } else {
      stackArgs[i] = Value(make<Object>(new WasmObjectValue(this, std::move(arg))));
    }
  }
  Value *args = stackArgs;

  auto thisVal = Value();

  try {
    if (!func) {
      std::cerr << "Func is not defined" << std::endl;
      throw std::logic_error("Func is not defined");
    }
    auto result = func(*this, std::move(thisVal), args, size);

    return toEmscriptenVal(result);
  } catch (std::exception error) {
    std::cerr << error.what() << std::endl;

    throw error;
  }
}

emscripten::val Runtime::invoke(std::string methodName, emscripten::val jsonArgs) {
  auto pair = runtimeMethods.find(methodName);

  if (pair != runtimeMethods.end()) {
    auto object = static_cast<const WasmObjectValue *>(pair->second);

    return invokeHostFunction(object->func, jsonArgs);
  }

  throw std::invalid_argument("No " + methodName + " method registered");
}

facebook::jsi::Runtime::PointerValue *Runtime::getPublicPointerValue(facebook::jsi::Pointer &pointer) {
  return getPointerValue(pointer);
}
const facebook::jsi::Runtime::PointerValue *Runtime::getPublicPointerValue(const facebook::jsi::Pointer &pointer) {
  return getPointerValue(pointer);
}
const facebook::jsi::Runtime::PointerValue *Runtime::getPublicPointerValue(const facebook::jsi::Value &value) {
  return getPointerValue(value);
}

} // namespace ReactNativeWasm

EMSCRIPTEN_BINDINGS(ReactWasmRuntime) {
  emscripten::class_<ReactNativeWasm::Runtime::HostObjectProxy>("HostObjectProxy")
    .smart_ptr<std::shared_ptr<ReactNativeWasm::Runtime::HostObjectProxy>>("HostObjectProxy")
    .function(
      "runFunction",
      emscripten::optional_override(
        [](ReactNativeWasm::Runtime::HostObjectProxy &self, std::string functionName, emscripten::val jsonArgs) {
          std::cout << "HostObjectProxy::get Trying to get function: '" << functionName << "'" << std::endl;

          auto propName = facebook::jsi::PropNameID::forAscii(self.runtime, functionName.c_str(), functionName.size());

          auto value = self.ho->get(self.runtime, propName);

          auto wasmValue = static_cast<const ReactNativeWasm::Runtime::WasmObjectValue *>(
            ReactNativeWasm::Runtime::getPublicPointerValue(value));

          if (!wasmValue->isFunction) {
            std::cerr << "Property is not a function" << std::endl;

            throw new std::invalid_argument("Property is not a function");
          }

          return self.runtime.invokeHostFunction(wasmValue->func, jsonArgs);
        }))
    .function(
      "get",
      emscripten::optional_override([](ReactNativeWasm::Runtime::HostObjectProxy &self, std::string propertyName) {
        std::cout << "HostObjectProxy::get Trying to get property: '" << propertyName << "'" << std::endl;

        auto propName = facebook::jsi::PropNameID::forAscii(self.runtime, propertyName.c_str(), propertyName.size());

        auto value = self.ho->get(self.runtime, propName);

        return self.runtime.toEmscriptenVal(value);
      }));

  emscripten::class_<ReactNativeWasm::Runtime>("Runtime")
    .smart_ptr<std::shared_ptr<ReactNativeWasm::Runtime>>("Runtime")
    .function("invoke", &ReactNativeWasm::Runtime::invoke);
}
