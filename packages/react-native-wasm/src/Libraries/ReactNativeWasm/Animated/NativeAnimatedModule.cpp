#include "NativeAnimatedModule.hpp"

#include <cxxreact/JsArgumentHelpers.h>
#include <folly/json.h>
#include <iostream>

namespace ReactNativeWasm {
const char *NativeAnimatedModule::Name = "NativeAnimatedModule";

auto NativeAnimatedModule::getName() -> std::string { return NativeAnimatedModule::Name; }

auto NativeAnimatedModule::getConstants() -> std::map<std::string, folly::dynamic> { return {}; }

auto NativeAnimatedModule::getMethods() -> std::vector<Method> {
  return {
    Method(
      "startOperationBatch",
      [](folly::dynamic /*args*/) {
        // std::cout << "NativeAnimatedModule::startOperationBatch" << std::endl;
      }),

    Method(
      "finishOperationBatch",
      [](folly::dynamic /*args*/) {
        // std::cout << "NativeAnimatedModule::finishOperationBatch" << std::endl;
      }),
    Method(
      "createAnimatedNode",
      [this](folly::dynamic args) {
        const auto tag = facebook::xplat::jsArgAsInt(args, 0);
        const auto config = facebook::xplat::jsArgAsObject(args, 1);

        // std::cout << "NativeAnimatedModule::createAnimatedNode" << std::endl;
      }),
    Method(
      "getValue",
      [this](folly::dynamic args, Callback saveValueCallback) {
        const auto animatedNodeTag = facebook::xplat::jsArgAsInt(args, 0);
        // std::cout << "NativeAnimatedModule::getValue" << std::endl;
      }),
    Method(
      "connectAnimatedNodeToView",
      [this](folly::dynamic args) {
        const auto tag = facebook::xplat::jsArgAsInt(args, 0);
        const auto viewTag = facebook::xplat::jsArgAsInt(args, 1);
        // std::cout << "NativeAnimatedModule::connectAnimatedNodeToView" << std::endl;
      }),
    Method(
      "disconnectAnimatedNodeFromView",
      [this](folly::dynamic args) {
        const auto tag = facebook::xplat::jsArgAsInt(args, 0);
        const auto viewTag = facebook::xplat::jsArgAsInt(args, 1);
        // std::cout << "NativeAnimatedModule::disconnectAnimatedNodeFromView" << std::endl;
      }),
    Method(
      "connectAnimatedNodes",
      [this](folly::dynamic args) {
        const auto parentTag = facebook::xplat::jsArgAsInt(args, 0);
        const auto childTag = facebook::xplat::jsArgAsInt(args, 1);
        // std::cout << "NativeAnimatedModule::connectAnimatedNodes" << std::endl;
      }),
    Method(
      "disconnectAnimatedNodes",
      [this](folly::dynamic args) {
        const auto parentTag = facebook::xplat::jsArgAsInt(args, 0);
        const auto childTag = facebook::xplat::jsArgAsInt(args, 1);
        // std::cout << "NativeAnimatedModule::disconnectAnimatedNodes" << std::endl;
      }),
    Method(
      "stopAnimation",
      [this](folly::dynamic args) {
        const auto animationId = facebook::xplat::jsArgAsInt(args, 0);
        // std::cout << "NativeAnimatedModule::stopAnimation" << std::endl;
      }),
    Method(
      "startAnimatingNode",
      [this](folly::dynamic args, Callback endCallback) {
        const auto animationId = facebook::xplat::jsArgAsInt(args, 0);
        const auto animatedNodeTag = facebook::xplat::jsArgAsInt(args, 1);
        const auto animationConfig = facebook::xplat::jsArgAsObject(args, 2);
        // std::cout << "NativeAnimatedModule::startAnimatingNode" << std::endl;
      }),
    Method(
      "dropAnimatedNode",
      [this](folly::dynamic args) {
        const auto tag = facebook::xplat::jsArgAsInt(args, 0);
        // std::cout << "NativeAnimatedModule::dropAnimatedNode" << std::endl;
      }),
    Method(
      "setAnimatedNodeValue",
      [this](folly::dynamic args) {
        const auto tag = facebook::xplat::jsArgAsInt(args, 0);
        const auto value = facebook::xplat::jsArgAsDouble(args, 1);
        // std::cout << "NativeAnimatedModule::setAnimatedNodeValue" << std::endl;
      }),
    Method(
      "setAnimatedNodeOffset",
      [this](folly::dynamic args) {
        const auto tag = facebook::xplat::jsArgAsInt(args, 0);
        const auto value = facebook::xplat::jsArgAsDouble(args, 1);
        // std::cout << "NativeAnimatedModule::setAnimatedNodeOffset" << std::endl;
      }),
    Method(
      "flattenAnimatedNodeOffset",
      [this](folly::dynamic args) {
        const auto tag = facebook::xplat::jsArgAsInt(args, 0);
        // std::cout << "NativeAnimatedModule::flattenAnimatedNodeOffset" << std::endl;
      }),
    Method(
      "extractAnimatedNodeOffset",
      [this](folly::dynamic args) {
        const auto tag = facebook::xplat::jsArgAsInt(args, 0);
        // std::cout << "NativeAnimatedModule::extractAnimatedNodeOffset" << std::endl;
      }),
    Method(
      "addAnimatedEventToView",
      [this](folly::dynamic args) {
        const auto viewTag = facebook::xplat::jsArgAsInt(args, 0);
        const auto eventName = facebook::xplat::jsArgAsString(args, 1);
        const auto eventMapping = facebook::xplat::jsArgAsObject(args, 2);
        // std::cout << "NativeAnimatedModule::addAnimatedEventToView" << std::endl;
      }),
    Method(
      "removeAnimatedEventFromView",
      [this](folly::dynamic args) {
        const auto viewTag = facebook::xplat::jsArgAsInt(args, 0);
        const auto eventName = facebook::xplat::jsArgAsString(args, 1);
        const auto animatedValueTag = facebook::xplat::jsArgAsInt(args, 2);
        // std::cout << "NativeAnimatedModule::removeAnimatedEventFromView" << std::endl;
      }),
    Method(
      "startListeningToAnimatedNodeValue",
      [this](folly::dynamic args) {
        const auto viewTag = facebook::xplat::jsArgAsInt(args, 0);
        // std::cout << "NativeAnimatedModule::startListeningToAnimatedNodeValue" << std::endl;
      }),
    Method(
      "stopListeningToAnimatedNodeValue",
      [this](folly::dynamic args) {
        const auto viewTag = facebook::xplat::jsArgAsInt(args, 0);
        // std::cout << "NativeAnimatedModule::stopListeningToAnimatedNodeValue" << std::endl;
      }),
  };
}
} // namespace ReactNativeWasm
