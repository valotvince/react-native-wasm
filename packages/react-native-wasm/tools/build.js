#!/usr/bin/env node

const path = require('path');

const spawnPromise = require('./spawn-promise');
const getCompilationManifest = require('./build/compilation-manifest');
const buildLibrary = require('./build/build-library');

const reactNativeWasmDir = path.resolve(__dirname, '..');
const reactNativeDir = path.resolve(path.join(process.cwd(), 'node_modules', 'react-native'));

const dependencies = [
  {
    name: 'fmt',
    version: '6.2.1',
    remote: 'https://github.com/fmtlib/fmt/releases/download/6.2.1/fmt-6.2.1.zip',
  },
];

(async () => {
  // await overrideRn();

  const { libraries, main } = getCompilationManifest(reactNativeWasmDir, reactNativeDir);

  await Promise.all(libraries.map((library) => buildLibrary(reactNativeWasmDir, library)));

  await spawnPromise(
    'emcc',
    [
      '-lembind',
      '-pthread',
      '-s',
      'USE_SDL=2',
      '-s',
      'LLD_REPORT_UNDEFINED=1',
      // '-s',
      // 'PROXY_TO_PTHREAD',
      '-sEXPORTED_RUNTIME_METHODS=ccall,cwrap',
      // Fast-incremental builds
      '-O0',
      '-std=c++17',
      '--js-library',
      path.join(reactNativeWasmDir, 'src/Libraries/Utilities/JavascriptAccessor/JavascriptAccessor.js'),

      ...main.definitions.map((definition) => `-D${definition}`),
      ...main.includes.map((include) => `-I${include}`),

      ...main.sources,

      ...libraries.map(({ name }) => `build/${name}/${name}.a`),

      '-o',
      'dist/index.html',
    ],
    { cwd: reactNativeWasmDir },
  );

  // await spawnPromise('react-native', [
  //   'bundle',
  //   '--platform',
  //   'wasm',
  //   '--entry-file',
  //   './index.tsx',
  //   '--dev',
  //   'true',
  //   '--bundle-output',
  //   path.resolve(path.join(reactNativeWasmDir, 'dist', 'react-native.bundle.js')),
  // ]);
})();
