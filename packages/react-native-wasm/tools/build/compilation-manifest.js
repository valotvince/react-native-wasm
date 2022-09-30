const path = require('path');
const fs = require('fs');

const getFilesFromDir = (dir, extension = '.cpp') => {
  return fs
    .readdirSync(dir)
    .filter((fileName) => fileName.endsWith(extension))
    .map((fileName) => path.join(dir, fileName));
};

const getFilesFromRootDir = (rootDir) => (subdir) => getFilesFromDir(path.join(rootDir, subdir), '.cpp');

const getFmt = (reactNativeWasmDir) => {
  const fmtDir = path.join(reactNativeWasmDir, 'deps', 'fmt');

  return {
    name: 'lib-fmt',
    compilerFlags: ['-pthread'],
    definitions: [],
    includes: [path.join(fmtDir, 'include')],
    sources: getFilesFromDir(path.join(fmtDir, 'src'), '.cc'),
  };
};

const getDoubleConversion = (reactNativeWasmDir) => {
  const doubleConversionDir = path.join(reactNativeWasmDir, 'deps', 'double-conversion', 'src');

  return {
    name: 'lib-doubleconversion',
    compilerFlags: ['-pthread'],
    definitions: ['__x86_64__'],
    includes: ['/usr/local/opt/glog/include', path.join(reactNativeWasmDir, 'deps', 'double-conversion', 'include')],
    sources: getFilesFromDir(doubleConversionDir, '.cc'),
  };
};

const getFolly = (reactNativeWasmDir) => {
  const follyDir = path.join(reactNativeWasmDir, 'deps', 'folly');

  return {
    name: 'lib-folly',
    compilerFlags: ['-pthread'],
    definitions: ['FOLLY_NO_CONFIG', 'FOLLY_HAVE_PTHREAD=1', 'FOLLY_HAVE_PTHREAD_ATFORK=1'],
    includes: [
      '/usr/local/opt/glog/include',
      '/usr/local/opt/gflags/include',
      path.join(reactNativeWasmDir, 'deps', 'boost'),
      follyDir,
    ],
    sources: [
      'folly/SharedMutex.cpp',
      'folly/concurrency/CacheLocality.cpp',
      'folly/detail/Futex.cpp',
      'folly/lang/SafeAssert.cpp',
      'folly/lang/ToAscii.cpp',
      'folly/lang/Assume.cpp',
      'folly/synchronization/ParkingLot.cpp',
      'folly/system/ThreadId.cpp',
      'folly/system/ThreadName.cpp',
      'folly/json.cpp',
      'folly/Unicode.cpp',
      'folly/Conv.cpp',
      'folly/Demangle.cpp',
      'folly/memory/detail/MallocImpl.cpp',
      'folly/String.cpp',
      'folly/dynamic.cpp',
      'folly/FileUtil.cpp',
      'folly/Format.cpp',
      'folly/net/NetOps.cpp',
      'folly/json_pointer.cpp',
      'folly/lang/CString.cpp',
      'folly/detail/UniqueInstance.cpp',
      'folly/hash/SpookyHashV2.cpp',
      'folly/container/detail/F14Table.cpp',
      'folly/ScopeGuard.cpp',
      'folly/portability/SysUio.cpp',
    ].map((file) => path.join(follyDir, file)),
  };
};

const getReactNative = (reactNativeWasmDir, reactNativeDir) => {
  const reactNativeReactCommonDir = path.join(reactNativeDir, 'ReactCommon');
  const reactNativeCxxReactPath = path.join(reactNativeReactCommonDir, 'cxxreact');

  const getFilesFromReactCommonDir = getFilesFromRootDir(reactNativeReactCommonDir);

  return {
    name: 'lib-reactnative',
    definitions: ['__unused=[[maybe_unused]]'],
    compilerFlags: ['-pthread'],
    includes: [
      path.join(reactNativeDir, 'ReactCommon'),
      ...[
        'react/renderer/graphics/platform/cxx',
        'react/nativemodule/core',
        'runtimeexecutor',
        'jsi',
        'callinvoker',
        'yoga',
        'reactperflogger',
        'react/nativemodule/samples',
      ].map((file) => path.join(reactNativeReactCommonDir, file)),
    ],
    sources: [
      ...getFilesFromReactCommonDir('react/renderer/components/view'),
      ...getFilesFromReactCommonDir('react/renderer/components/root'),
      ...getFilesFromReactCommonDir('react/renderer/uimanager'),
      ...getFilesFromReactCommonDir('react/renderer/componentregistry'),
      ...getFilesFromReactCommonDir('react/renderer/mounting'),
      ...getFilesFromReactCommonDir('react/renderer/leakchecker'),
      ...getFilesFromReactCommonDir('react/renderer/core'),
      ...getFilesFromReactCommonDir('react/renderer/scheduler'),
      ...getFilesFromReactCommonDir('react/renderer/debug'),
      ...getFilesFromReactCommonDir('react/renderer/graphics'),
      ...getFilesFromReactCommonDir('react/renderer/graphics/platform/cxx/react/renderer/graphics'),
      ...getFilesFromReactCommonDir('react/renderer/runtimescheduler'),
      ...getFilesFromReactCommonDir('react/renderer/telemetry'),
      ...getFilesFromReactCommonDir('react/utils'),
      ...getFilesFromReactCommonDir('react/config'),
      ...getFilesFromReactCommonDir('jsi/jsi'),
      ...getFilesFromReactCommonDir('yoga/yoga'),
      ...getFilesFromReactCommonDir('yoga/yoga/event'),
      ...getFilesFromReactCommonDir('yoga/yoga/internal'),
      ...[
        'react/bridging/LongLivedObject.cpp',
        'react/nativemodule/samples/ReactCommon/SampleTurboCxxModule.cpp',
        'react/nativemodule/core/ReactCommon/TurboModule.cpp',
        'react/nativemodule/core/ReactCommon/TurboModuleBinding.cpp',
        'reactperflogger/reactperflogger/BridgeNativeModulePerfLogger.cpp',
        'logger/react_native_log.cpp',
      ].map((file) => path.join(reactNativeReactCommonDir, file)),
      ...[
        'MethodCall.cpp',
        'Instance.cpp',
        'CxxNativeModule.cpp',
        'ModuleRegistry.cpp',
        'NativeToJsBridge.cpp',
        'JSBigString.cpp',
      ].map((file) => path.join(reactNativeCxxReactPath, file)),
    ],
  };
};

const getReactNativeWasm = (reactNativeWasmDir) => {
  const getFilesFromReactNativeWasmDir = getFilesFromRootDir(path.join(reactNativeWasmDir, 'src'));

  return {
    name: 'app',
    compilerFlags: ['-pthread', '-s', 'USE_SDL=2', '-fexceptions'],
    includes: [],
    definitions: [],
    sources: [
      ...getFilesFromReactNativeWasmDir('Libraries/ReactNativeWasm/UIManager'),
      ...getFilesFromReactNativeWasmDir('Libraries/Components/View'),
      ...getFilesFromReactNativeWasmDir('Libraries/Components/Text'),
      ...getFilesFromReactNativeWasmDir('Libraries/ReactNativeWasm/Config'),
      ...[
        'ReactWasmEntry.cpp',
        'ReactWasmInstance.cpp',
        'Libraries/Utilities/Timing/Timing.cpp',
        'Libraries/Utilities/DevSettings/DevSettings.cpp',
        'Libraries/Utilities/PlatformConstants.cpp',
        'Libraries/ReactNativeWasm/Scheduler/SchedulerDelegate.cpp',
        'Libraries/ReactNativeWasm/NativeQueue/NativeQueue.cpp',
        'Libraries/ReactNativeWasm/Renderer/Renderer.cpp',
        'Libraries/ReactNativeWasm/Runtime/Runtime.cpp',
        'Libraries/ReactNativeWasm/Bindings/JSWasmExecutor.cpp',
      ].map((file) => path.join(reactNativeWasmDir, 'src', file)),
    ],
  };
};

const mergeValues = (library, key, values) => ({
  ...library,
  [key]: Array.from(new Set([...values, ...library[key]])),
});

const prependValues = (libraries, key, values) => libraries.map((library) => mergeValues(library, key, values));

const imbricatedValues = (libraryIncludesChain, key) =>
  libraryIncludesChain.reduce((accumulator, library, index) => {
    const previousLibrary = accumulator[index - 1];

    if (!previousLibrary) {
      accumulator.push(library);

      return accumulator;
    }

    accumulator.push(mergeValues(library, key, previousLibrary[key]));

    return accumulator;
  }, []);

module.exports = (reactNativeWasmDir, reactNativeDir) => {
  const libraries = imbricatedValues(
    prependValues(
      imbricatedValues(
        [
          getDoubleConversion(reactNativeWasmDir),
          getFmt(reactNativeWasmDir),
          getFolly(reactNativeWasmDir),
          getReactNative(reactNativeWasmDir, reactNativeDir),
          getReactNativeWasm(reactNativeWasmDir),
        ],
        'includes',
      ),
      'includes',
      [path.join(reactNativeWasmDir, 'src', 'stubs')],
    ),
    'definitions',
  );

  return {
    libraries,
  };
};
