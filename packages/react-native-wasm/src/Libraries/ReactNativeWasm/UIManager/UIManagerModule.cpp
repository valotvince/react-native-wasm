#include "UIManagerModule.hpp"

#include <iostream>
#include <folly/json.h>
#include <cxxreact/JsArgumentHelpers.h>

namespace ReactNativeWasm {
    const char* UIManagerModule::Name = "UIManager";

    auto UIManagerModule::getName() -> std::string {
        return UIManagerModule::Name;
    }

    auto UIManagerModule::getConstants() -> std::map<std::string, folly::dynamic> {
        return {};
    }

    auto UIManagerModule::getManager(std::string viewManagerName) -> std::shared_ptr<ReactNativeWasm::Components::Manager> {
        for (auto it = this->componentManagers->begin(); it != this->componentManagers->end(); ++it) {
            if (it->get()->getName() == viewManagerName) {
                return this->componentManagers->at(std::distance(this->componentManagers->begin(), it));
            }
        }

        std::cout << "No ViewManager by the name of " << viewManagerName << std::endl;

        throw std::invalid_argument("No ViewManager by the name of " + viewManagerName);
    }

    auto UIManagerModule::getMethods() -> std::vector<Method> {
        return {
            Method(
                "getConstantsForViewManager",
                [this](folly::dynamic args) {
                    auto viewManagerName = facebook::xplat::jsArgAsString(args, 0);

                    auto manager = this->getManager(viewManagerName);

                    return manager->getConstants();
                },
                CxxModule::SyncTag
            ),
            Method(
                "lazilyLoadView",
                [this](folly::dynamic args) {
                    auto viewName = facebook::xplat::jsArgAsString(args, 0);

                    return folly::dynamic::object();
                },
                CxxModule::SyncTag
            ),
            Method(
                "createView",
                [this](folly::dynamic args) {
                    auto reactTag = facebook::xplat::jsArgAsInt(args, 0);
                    auto viewName = facebook::xplat::jsArgAsString(args, 1);
                    auto rootTag = facebook::xplat::jsArgAsInt(args, 2);
                    auto props = facebook::xplat::jsArgAsDynamic(args, 3);

                    std::cout << "UIManagerModule::createView reactTag:" << std::to_string(reactTag) << " viewName:" << viewName << " rootTag:" << std::to_string(rootTag) << " props:" << folly::toJson(props).c_str() << std::endl;

                    auto viewManager = this->getManager(viewName);

                    auto node = viewManager->createShadow();
                    node->className = std::move(viewName);
                    node->tag = reactTag;
                    node->rootTag = rootTag;
                    // node->viewManager = viewManager;

                    node->createView(std::move(props));

                    this->renderer->render(node);

                    std::cout << "UIManagerModule::UIManagerModule nodes size 2 " << std::endl;
                    std::cout << std::to_string(this->nodes->size()) << std::endl;

                    this->nodes->insert(std::make_pair(reactTag, node));
                }
            ),
            Method(
                "manageChildren",
                [this](folly::dynamic args) {
                    auto containerTag = facebook::xplat::jsArgAsInt(args, 0);
                    auto moveFromIndices = facebook::xplat::jsArgAsArray(args, 1);
                    auto moveToIndices = facebook::xplat::jsArgAsArray(args, 2);
                    auto addChildReactTags = facebook::xplat::jsArgAsArray(args, 3);
                    auto addAtIndices = facebook::xplat::jsArgAsArray(args, 4);
                    auto removeAtIndices = facebook::xplat::jsArgAsArray(args, 5);
                }
            ),
            Method(
                "setChildren",
                [this](folly::dynamic args) {
                    auto containerTag = facebook::xplat::jsArgAsInt(args, 0);
                    auto reactTags = facebook::xplat::jsArgAsArray(args, 1);

                    std::cout << "UIManagerModule::setChildren 2 " << folly::toJson(args) << std::endl;
                }
            ),
            Method(
                "updateView",
                [this](folly::dynamic args) {
                    auto reactTag = facebook::xplat::jsArgAsInt(args, 0);
                    auto viewName = facebook::xplat::jsArgAsString(args, 1);
                    auto props = facebook::xplat::jsArgAsDynamic(args, 2);

                    std::cout << "UIManagerModule::updateView " << folly::toJson(args) << std::endl;

                    auto node = this->nodes->find(reactTag);

                    if (node != this->nodes->end()) {
                        node->second->getView()->props = std::move(props);

                        this->renderer->render(node->second);
                    }
                }
            ),
            Method(
                "focus",
                [this](folly::dynamic args) {
                    auto reactTag = facebook::xplat::jsArgAsInt(args, 0);
                }
            ),
            Method(
                "blur",
                [this](folly::dynamic args) {
                    auto reactTag = facebook::xplat::jsArgAsInt(args, 0);
                }
            ),
            Method(
                "findSubviewIn",
                [this](folly::dynamic args, Callback callback) {
                    auto reactTag = facebook::xplat::jsArgAsInt(args, 0);
                    auto point = facebook::xplat::jsArgAsArray(args, 1);
                }
            ),
            Method(
                "dispatchViewManagerCommand",
                [this](folly::dynamic args, Callback callback) {
                    auto reactTag = facebook::xplat::jsArgAsInt(args, 0);
                    auto commandId = facebook::xplat::jsArgAsInt(args, 1);
                    auto commandArgs = facebook::xplat::jsArgAsArray(args, 2);
                }
            ),
            Method(
                "measure",
                [this](folly::dynamic args, Callback callback) {
                    auto reactTag = facebook::xplat::jsArgAsInt(args, 0);
                }
            ),
            Method(
                "measureInWindow",
                [this](folly::dynamic args, Callback callback) {
                    auto reactTag = facebook::xplat::jsArgAsInt(args, 0);
                }
            ),
            Method(
                "viewIsDescendantOf",
                [this](folly::dynamic args, Callback callback) {
                    auto reactTag = facebook::xplat::jsArgAsInt(args, 0);
                    auto ancestorReactTag = facebook::xplat::jsArgAsInt(args, 1);
                }
            ),
            Method(
                "measureLayout",
                [this](folly::dynamic args, Callback errorCallback, Callback callback) {
                    auto reactTag = facebook::xplat::jsArgAsInt(args, 0);
                    auto ancestorReactTag = facebook::xplat::jsArgAsInt(args, 1);
                }
            ),
        };
    }
}
