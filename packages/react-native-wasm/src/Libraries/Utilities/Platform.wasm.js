import * as TurboModuleRegistry from '../TurboModule/TurboModuleRegistry';

const PlatformConstants = TurboModuleRegistry.getEnforcing('PlatformConstants');

export default {
  __constants: null,
  OS: 'wasm',
  get Version() {
    return 1;
  },
  get isTesting() {
    return false;
  },
  get constants() {
    if (this.__constants == null) {
      this.__constants = PlatformConstants.getConstants();
    }
    return this.__constants;
  },
  select: (spec) => ('wasm' in spec ? spec.wasm : 'native' in spec ? spec.native : spec.default),
};
