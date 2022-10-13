#include "SchedulerDelegate.hpp"

#include <react/renderer/mounting/MountingTransaction.h>
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
      //[self.delegate mountingManager:self willMountComponentsWithRootTag:surfaceId];
      // _observerCoordinator.notifyObserversMountingTransactionWillMount(transaction, surfaceTelemetry);
    },
    [&](
      facebook::react::MountingTransaction const &transaction,
      facebook::react::SurfaceTelemetry const &surfaceTelemetry) {
      // RCTPerformMountInstructions(
      //     transaction.getMutations(), /* _componentViewRegistry, _observerCoordinator,*/ surfaceId);
    },
    [&](
      facebook::react::MountingTransaction const &transaction,
      facebook::react::SurfaceTelemetry const &surfaceTelemetry) {
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
