#include "SchedulerDelegate.hpp"

#include <iostream>
#include <react/renderer/mounting/MountingTransaction.h>
#include <react/renderer/mounting/ShadowViewMutation.h>
#include <react/renderer/telemetry/SurfaceTelemetry.h>

namespace ReactNativeWasm {
/*
 * Called right after Scheduler computed (and laid out) a new updated version
 * of the tree and calculated a set of mutations which are sufficient
 * to construct a new one.
 */
void SchedulerDelegate::schedulerDidFinishTransaction(
  facebook::react::MountingCoordinator::Shared const &mountingCoordinator) {
  std::cout << "SchedulerDelegate::schedulerDidFinishTransaction" << std::endl;

  auto surfaceId = mountingCoordinator->getSurfaceId();

  mountingCoordinator->getTelemetryController().pullTransaction(
    [&](
      facebook::react::MountingTransaction const &transaction,
      facebook::react::SurfaceTelemetry const &surfaceTelemetry) {
      std::cout << "SchedulerDelegate::schedulerDidFinishTransaction pullTransaction willMount" << std::endl;

      //[self.delegate mountingManager:self willMountComponentsWithRootTag:surfaceId];
      // _observerCoordinator.notifyObserversMountingTransactionWillMount(transaction, surfaceTelemetry);
    },
    [&](
      facebook::react::MountingTransaction const &transaction,
      facebook::react::SurfaceTelemetry const &surfaceTelemetry) {
      std::cout << "SchedulerDelegate::schedulerDidFinishTransaction pullTransaction mount" << std::endl;

      auto mutations = transaction.getMutations();

      for (auto const &mutation : mutations) {
        const auto &parentShadowView = mutation.parentShadowView;
        const auto &oldChildShadowView = mutation.oldChildShadowView;
        const auto &newChildShadowView = mutation.newChildShadowView;

        switch (mutation.type) {
        case facebook::react::ShadowViewMutation::Create: {
          std::cout << "facebook::react::ShadowViewMutation::Create " << newChildShadowView.componentName << std::endl;

          auto contentFrame = newChildShadowView.layoutMetrics.frame;

          std::cout << "x:" << contentFrame.origin.x << " y:" << contentFrame.origin.y
                    << " width:" << contentFrame.size.width << " height:" << contentFrame.size.height << std::endl;
          break;
        }

        case facebook::react::ShadowViewMutation::Delete: {
          std::cout << "facebook::react::ShadowViewMutation::Delete" << std::endl;
          auto contentFrame = newChildShadowView.layoutMetrics.frame;

          std::cout << "x:" << contentFrame.origin.x << " y:" << contentFrame.origin.y
                    << " width:" << contentFrame.size.width << " height:" << contentFrame.size.height << std::endl;
          break;
        }

        case facebook::react::ShadowViewMutation::Insert: {
          std::cout << "facebook::react::ShadowViewMutation::Insert " << newChildShadowView.componentName << std::endl;

          renderer->render(newChildShadowView);
          break;
        }

        case facebook::react::ShadowViewMutation::Remove: {
          std::cout << "facebook::react::ShadowViewMutation::Remove" << std::endl;
          break;
        }

        case facebook::react::ShadowViewMutation::RemoveDeleteTree: {
          std::cout << "facebook::react::ShadowViewMutation::RemoveDeleteTree" << std::endl;
          break;
        }

        case facebook::react::ShadowViewMutation::Update: {
          std::cout << "facebook::react::ShadowViewMutation::Update " << newChildShadowView.componentName << std::endl;
          auto contentFrame = newChildShadowView.layoutMetrics.frame;

          std::cout << "x:" << contentFrame.origin.x << " y:" << contentFrame.origin.y
                    << " width:" << contentFrame.size.width << " height:" << contentFrame.size.height << std::endl;
          break;
        }
        }
      }

      // RCTPerformMountInstructions(
      //     transaction.getMutations(), /* _componentViewRegistry, _observerCoordinator,*/ surfaceId);
    },
    [&](
      facebook::react::MountingTransaction const &transaction,
      facebook::react::SurfaceTelemetry const &surfaceTelemetry) {
      std::cout << "SchedulerDelegate::schedulerDidFinishTransaction pullTransaction didMount" << std::endl;

      //_observerCoordinator.notifyObserversMountingTransactionDidMount(transaction, surfaceTelemetry);
      // didMountComponentsWithRootTag(surfaceId);
      //[self.delegate mountingManager:self didMountComponentsWithRootTag:surfaceId];
    });
};

/*
 * Called right after a new ShadowNode was created.
 */
void SchedulerDelegate::schedulerDidRequestPreliminaryViewAllocation(
  facebook::react::SurfaceId surfaceId, const facebook::react::ShadowNode &shadowView) {
  std::cout << "SchedulerDelegate::schedulerDidRequestPreliminaryViewAllocation" << std::endl;
};

/*
 * Called right after a ShadowNode is cloned.
 */
void SchedulerDelegate::schedulerDidCloneShadowNode(
  facebook::react::SurfaceId surfaceId, const facebook::react::ShadowNode &oldShadowNode,
  const facebook::react::ShadowNode &newShadowNode) {
  std::cout << "SchedulerDelegate::schedulerDidCloneShadowNode" << std::endl;
};

void SchedulerDelegate::schedulerDidDispatchCommand(
  const facebook::react::ShadowView &shadowView, std::string const &commandName, folly::dynamic const &args) {
  std::cout << "SchedulerDelegate::schedulerDidDispatchCommand" << std::endl;
};

void SchedulerDelegate::schedulerDidSendAccessibilityEvent(
  const facebook::react::ShadowView &shadowView, std::string const &eventType) {
  std::cout << "SchedulerDelegate::schedulerDidSendAccessibilityEvent" << std::endl;
};

/*
 * Set JS responder for a view
 */
void SchedulerDelegate::schedulerDidSetIsJSResponder(
  facebook::react::ShadowView const &shadowView, bool isJSResponder, bool blockNativeResponder) {
  std::cout << "SchedulerDelegate::schedulerDidSetIsJSResponder" << std::endl;
};

SchedulerDelegate::~SchedulerDelegate(){};
} // namespace ReactNativeWasm
