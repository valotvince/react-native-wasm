/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include "../Fabric/ComponentView.hpp"
#include "../Renderer/Renderer.hpp"
#include <react/renderer/scheduler/SchedulerDelegate.h>

namespace ReactNativeWasm {

/*
 * Abstract class for Scheduler's delegate.
 */
class SchedulerDelegate : public facebook::react::SchedulerDelegate {
public:
  SchedulerDelegate(ReactNativeWasm::Renderer::Shared renderer);

  /*
   * Called right after Scheduler computed (and laid out) a new updated version
   * of the tree and calculated a set of mutations which are sufficient
   * to construct a new one.
   */
  void schedulerDidFinishTransaction(facebook::react::MountingCoordinator::Shared const &mountingCoordinator) override;

  /*
   * Called right after a new ShadowNode was created.
   */
  void schedulerDidRequestPreliminaryViewAllocation(
    facebook::react::SurfaceId surfaceId, const facebook::react::ShadowNode &shadowView) override;

  /*
   * Called right after a ShadowNode is cloned.
   */
  void schedulerDidCloneShadowNode(
    facebook::react::SurfaceId surfaceId, const facebook::react::ShadowNode &oldShadowNode,
    const facebook::react::ShadowNode &newShadowNode) override;

  void schedulerDidDispatchCommand(
    const facebook::react::ShadowView &shadowView, std::string const &commandName, folly::dynamic const &args) override;

  void schedulerDidSendAccessibilityEvent(
    const facebook::react::ShadowView &shadowView, std::string const &eventType) override;

  /*
   * Set JS responder for a view
   */
  void schedulerDidSetIsJSResponder(
    facebook::react::ShadowView const &shadowView, bool isJSResponder, bool blockNativeResponder) override;

  ~SchedulerDelegate() override;

private:
  ReactNativeWasm::Renderer::Shared renderer;
  std::map<facebook::react::Tag, ReactNativeWasm::ComponentView::Shared> views;
};
} // namespace ReactNativeWasm
