#include "SchedulerDelegate.hpp"

namespace ReactNativeWasm {
  /*
   * Called right after Scheduler computed (and laid out) a new updated version
   * of the tree and calculated a set of mutations which are sufficient
   * to construct a new one.
   */
void SchedulerDelegate::schedulerDidFinishTransaction(
      facebook::react::MountingCoordinator::Shared const &mountingCoordinator) {};

  /*
   * Called right after a new ShadowNode was created.
   */
    void SchedulerDelegate::schedulerDidRequestPreliminaryViewAllocation(
      facebook::react::SurfaceId surfaceId,
      const facebook::react::ShadowNode &shadowView) {};

  /*
   * Called right after a ShadowNode is cloned.
   */
    void SchedulerDelegate::schedulerDidCloneShadowNode(
      facebook::react::SurfaceId surfaceId,
      const facebook::react::ShadowNode &oldShadowNode,
      const facebook::react::ShadowNode &newShadowNode) {};

  void SchedulerDelegate::schedulerDidDispatchCommand(
      const facebook::react::ShadowView &shadowView,
      std::string const &commandName,
      folly::dynamic const &args) {};

  void SchedulerDelegate::schedulerDidSendAccessibilityEvent(
      const facebook::react::ShadowView &shadowView,
      std::string const &eventType) {};

  /*
   * Set JS responder for a view
   */
   void SchedulerDelegate::schedulerDidSetIsJSResponder(
      facebook::react::ShadowView const &shadowView,
      bool isJSResponder,
      bool blockNativeResponder) {};

 SchedulerDelegate::~SchedulerDelegate() {};
}
