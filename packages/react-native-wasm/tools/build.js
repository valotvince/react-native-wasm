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

  const { libraries } = getCompilationManifest(reactNativeWasmDir, reactNativeDir);

  await Promise.all(libraries.map((library) => buildLibrary(reactNativeWasmDir, library)));

  const warnings = ['all'].map((warning) => `-W${warning}`);
  const options = [
    'USE_SDL=2',
    'USE_SDL_TTF=2',
    'USE_SDL_IMAGE=2',
    'LLD_REPORT_UNDEFINED=1',
    'PTHREAD_POOL_SIZE=3',
    'USE_PTHREADS=1',
    'ALLOW_BLOCKING_ON_MAIN_THREAD=0',
    // 'PROXY_TO_PTHREAD',
    'EXPORTED_RUNTIME_METHODS=ccall,cwrap',
    'NO_EXIT_RUNTIME=1',
    'WARN_UNALIGNED=1',
    'ASSERTIONS=2',
    // 'ASYNCIFY',
    'ALLOW_MEMORY_GROWTH=1',
    'STACK_OVERFLOW_CHECK=1',
    // 'MIN_WEBGL_VERSION=2',
    // 'MAX_WEBGL_VERSION=2',
  ].map((warning) => `-s${warning}`);

  await spawnPromise(
    'emcc',
    [
      '-lembind',
      '-pthread',
      ...options,
      ...warnings,
      // Fast-incremental builds
      '-fexceptions',
      '-fsanitize=address',
      '-g',
      '-O0',
      '-std=c++17',
      // '--threadprofiler',
      '--js-library',
      path.join(reactNativeWasmDir, 'src/Libraries/Utilities/JavascriptAccessor/JavascriptAccessor.js'),
      '--preload-file',
      'Resources',
      '--use-preload-plugins',

      ...libraries.map(({ name }) => `build/${name}/${name}.a`),

      '-o',
      'dist/index.html',
    ],
    { cwd: reactNativeWasmDir },
  );

  await spawnPromise('react-native', [
    'bundle',
    '--platform',
    'wasm',
    '--entry-file',
    './index.tsx',
    '--dev',
    'true',
    '--bundle-output',
    path.resolve(path.join(reactNativeWasmDir, 'dist', 'react-native.bundle.js')),
  ]);
})();
