const path = require('path');

const spawnPromise = require('./spawn-promise');
const getCompilationManifest = require('./build/compilation-manifest');
const buildLibrary = require('./build/build-library');

module.exports = async ({ debug, appDir, reactNativeWasmDir, reactNativeDir }) => {
  const { libraries } = getCompilationManifest(reactNativeWasmDir, reactNativeDir);

  for (const library of libraries) {
    await buildLibrary(reactNativeWasmDir, library);
  }

  const warnings = ['all'].map((warning) => `-W${warning}`);
  const options = [
    'USE_SDL=2',
    'USE_SDL_TTF=2',
    // 'USE_SDL_IMAGE=2',
    'MIN_WEBGL_VERSION=2',
    'MAX_WEBGL_VERSION=2',
    // Allow usage of SDL outside of main thread
    'OFFSCREEN_FRAMEBUFFER=1',
    // 'OFFSCREENCANVAS_SUPPORT=1',

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
    'EXCEPTION_DEBUG=1',
  ].map((warning) => `-s${warning}`);

  if (debug) {
  }

  await spawnPromise(
    'emcc',
    [
      '-lembind',
      '-pthread',
      ...options,
      ...warnings,
      // Fast-incremental builds
      '-fexceptions',
      // '-fwasm-exceptions',
      '-fsanitize=address',
      '-g',
      '-O0',
      '-std=c++17',
      // '--threadprofiler',
      '--pre-js',
      path.join(reactNativeWasmDir, 'src/Libraries/Utilities/JavascriptAccessor/pre.js'),
      '--js-library',
      path.join(reactNativeWasmDir, 'src/Libraries/Utilities/JavascriptAccessor/library.js'),
      '--preload-file',
      'Resources',
      '--use-preload-plugins',
      // '--profiling-funcs',

      ...libraries.map(({ name }) => `build/${name}/${name}.a`),

      '-o',
      'dist/index.html',
    ],
    { cwd: reactNativeWasmDir },
  );

  await spawnPromise('react-native', [
    'bundle',
    '--config',
    path.join(reactNativeWasmDir, 'metro.config.js'),
    '--platform',
    'wasm',
    '--entry-file',
    path.join(appDir, 'index.tsx'),
    '--dev',
    'true',
    '--bundle-output',
    path.resolve(path.join(reactNativeWasmDir, 'dist', 'react-native.bundle.js')),
  ]);
};
