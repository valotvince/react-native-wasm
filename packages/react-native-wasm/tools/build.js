const path = require('path');

const spawnPromise = require('./spawn-promise');
const getCompilationManifest = require('./build/compilation-manifest');
const buildLibrary = require('./build/build-library');

module.exports = async ({ appDir, reactNativeWasmDir, reactNativeDir, buildConfig }) => {
  const { debug = false, initialMemory = 100 } = buildConfig;

  console.log(buildConfig);

  const { libraries } = getCompilationManifest(reactNativeWasmDir, reactNativeDir);

  for (const library of libraries) {
    await buildLibrary(reactNativeWasmDir, library, buildConfig);
  }

  const options = [
    // MAINS
    'NO_EXIT_RUNTIME=1',
    // 'EXPORTED_RUNTIME_METHODS=ccall,cwrap',

    // DEPS
    'USE_SDL=2',
    'USE_SDL_TTF=2',
    // 'USE_SDL_IMAGE=2',
    // Allow usage of SDL outside of main thread
    'OFFSCREEN_FRAMEBUFFER=1',
    // 'OFFSCREENCANVAS_SUPPORT=1',
    'MIN_WEBGL_VERSION=2',
    'MAX_WEBGL_VERSION=2',

    // THREADING
    'USE_PTHREADS=1',
    'PTHREAD_POOL_SIZE=3',
    'ALLOW_BLOCKING_ON_MAIN_THREAD=0',
    // 'PROXY_TO_PTHREAD',

    // MEMORY
    // Should be removed once the build is stable ? Make it configurable per the developer
    'ALLOW_MEMORY_GROWTH=1',
    // Default being 16MB
    `INITIAL_MEMORY=${initialMemory * 1024 * 1024}`,

    // APP SIZE
    // TODO Performance: See how it be reduced on specific platforms
    'INCLUDE_FULL_LIBRARY=1',
    'ENVIRONMENT=web,worker',

    // OTHERS
    'LLD_REPORT_UNDEFINED=1',
    'WARN_UNALIGNED=1',
    'ASSERTIONS=2',
    // 'ASYNCIFY',
    'STACK_OVERFLOW_CHECK=1',
    'EXCEPTION_DEBUG=1',
  ].map((warning) => `-s${warning}`);

  if (debug) {
    options.push(
      '-g',
      '-O0',
      '-fexceptions',
      '-fsanitize=address',
      // '-fwasm-exceptions',
      // '--threadprofiler',
      // '--profiling-funcs'
    );
  } else {
    options.push('-O2');
  }

  await spawnPromise(
    'emcc',
    [
      '-std=c++17',
      '-lembind',
      '-pthread',

      ...options,
      '-Wall',

      '--pre-js',
      path.join(reactNativeWasmDir, 'src/Libraries/Utilities/JavascriptAccessor/pre.js'),
      '--js-library',
      path.join(reactNativeWasmDir, 'src/Libraries/Utilities/JavascriptAccessor/library.js'),
      '--preload-file',
      'Resources',
      '--use-preload-plugins',

      ...libraries.map(({ name }) => path.join('build', name, debug ? 'debug' : 'release', `${name}.a`)),

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
    debug ? 'true' : 'false',
    '--bundle-output',
    path.resolve(path.join(reactNativeWasmDir, 'dist', 'react-native.bundle.js')),
  ]);
};
