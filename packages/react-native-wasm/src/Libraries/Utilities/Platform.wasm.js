module.exports = {
  __constants: null,
  OS: 'wasm',
  get Version() {
    return 1;
  },
  get isTesting() {
    return false;
  },
  select: (spec) => ('wasm' in spec ? spec.wasm : 'native' in spec ? spec.native : spec.default),
};
