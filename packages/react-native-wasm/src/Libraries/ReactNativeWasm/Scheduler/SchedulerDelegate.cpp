#include "SchedulerDelegate.hpp"

#include <iostream>
#include <react/renderer/mounting/MountingTransaction.h>
#include <react/renderer/mounting/ShadowViewMutation.h>
#include <react/renderer/telemetry/SurfaceTelemetry.h>

namespace ReactNativeWasm {

SchedulerDelegate::SchedulerDelegate(ReactNativeWasm::Renderer::Shared rendererArg) {
  renderer = rendererArg;
  views = std::map<facebook::react::Tag, ReactNativeWasm::ComponentView::Shared>();

  // TODO: Remove hack to have the Root view already registered at startup
  views.insert({11, std::make_shared<ComponentView>()});
};

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

          views.insert({newChildShadowView.tag, std::make_shared<ComponentView>()});
          break;
        }

        case facebook::react::ShadowViewMutation::Delete: {
          std::cout << "facebook::react::ShadowViewMutation::Delete" << std::endl;
          views.erase(oldChildShadowView.tag);
          break;
        }

        case facebook::react::ShadowViewMutation::Insert: {
          std::cout << "facebook::react::ShadowViewMutation::Insert " << newChildShadowView.tag << ":"
                    << newChildShadowView.componentName << std::endl;

          auto parentLookup = views.find(parentShadowView.tag);
          auto childLookup = views.find(newChildShadowView.tag);

          childLookup->second->update(newChildShadowView);
          parentLookup->second->addChild(newChildShadowView.tag, childLookup->second);
          break;
        }

        case facebook::react::ShadowViewMutation::Remove: {
          std::cout << "facebook::react::ShadowViewMutation::Remove" << std::endl;

          auto parentLookup = views.find(parentShadowView.tag);

          parentLookup->second->removeChild(oldChildShadowView.tag);
          break;
        }

        case facebook::react::ShadowViewMutation::RemoveDeleteTree: {
          std::cout << "facebook::react::ShadowViewMutation::RemoveDeleteTree" << std::endl;
          break;
        }

        case facebook::react::ShadowViewMutation::Update: {
          std::cout << "facebook::react::ShadowViewMutation::Update " << newChildShadowView.tag << ":"
                    << newChildShadowView.componentName << std::endl;

          auto childLookup = views.find(newChildShadowView.tag);
          childLookup->second->update(newChildShadowView);

          break;
        }
        }
      }

      // TODO Perf: Don't render everything at once, only re-render changes
      auto rootLookup = views.find(11);
      renderer->render(rootLookup->second);

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
