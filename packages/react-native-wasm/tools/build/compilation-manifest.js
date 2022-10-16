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
    includes: ['/usr/local/opt/glog/include'],
    sources: getFilesFromDir(doubleConversionDir, '.cc'),
  };
};

const getFolly = (reactNativeWasmDir) => {
  const follyDir = path.join(reactNativeWasmDir, 'deps', 'folly');

  return {
    name: 'lib-folly',
    compilerFlags: ['-pthread'],
    definitions: [
      'FOLLY_NO_CONFIG',
      'FOLLY_HAVE_PTHREAD=1',
      'FOLLY_HAVE_PTHREAD_ATFORK=1',
      'FOLLY_MOBILE=0',
      'FOLLY_USE_LIBCPP=1',
    ],
    includes: [
      '/usr/local/opt/glog/include',
      '/usr/local/opt/boost/include',
      '/usr/local/opt/gflags/include',
      '/usr/local/opt/fmt/include',
      '/usr/local/opt/double-conversion/include',

      path.join(reactNativeWasmDir, 'deps'),
    ],
    sources: [
      path.join(reactNativeWasmDir, 'src', 'stubs', 'folly', 'system', 'ThreadId.cpp'),
      ...[
        'SharedMutex.cpp',
        'concurrency/CacheLocality.cpp',
        'String.cpp',
        'Conv.cpp',
        'Demangle.cpp',
        'FileUtil.cpp',
        'Format.cpp',
        'lang/Assume.cpp',
        'lang/CString.cpp',
        'lang/Exception.cpp',
        'lang/SafeAssert.cpp',
        'lang/ToAscii.cpp',
        'ScopeGuard.cpp',
        'Unicode.cpp',
        'dynamic.cpp',
        'json.cpp',
        'json_pointer.cpp',
        'container/detail/F14Table.cpp',
        'detail/Futex.cpp',
        'detail/UniqueInstance.cpp',
        'hash/SpookyHashV2.cpp',
        'memory/detail/MallocImpl.cpp',
        'net/NetOps.cpp',
        'portability/SysUio.cpp',
        'portability/SysResource.cpp',
        'portability/Unistd.cpp',
        'synchronization/ParkingLot.cpp',
        'synchronization/SanitizeThread.cpp',
      ].map((file) => path.join(follyDir, file)),
    ],
  };
};

const getReactNative = (reactNativeWasmDir, reactNativeDir) => {
  const reactNativeReactCommonDir = path.join(reactNativeDir, 'ReactCommon');
  const reactNativeCxxReactPath = path.join(reactNativeReactCommonDir, 'cxxreact');

  const getFilesFromReactCommonDir = getFilesFromRootDir(reactNativeReactCommonDir);
  const getFilesFromWasmReactCommonDir = getFilesFromRootDir(path.join(reactNativeWasmDir, 'src', 'ReactCommon'));

  return {
    name: 'lib-reactnative',
    definitions: ['__unused=[[maybe_unused]]'],
    compilerFlags: ['-pthread', '-fexceptions'],
    includes: [
      '/usr/local/opt/glog/include',
      '/usr/local/opt/boost/include',
      '/usr/local/opt/gflags/include',
      '/usr/local/opt/fmt/include',
      '/usr/local/opt/double-conversion/include',
      path.join(reactNativeWasmDir, 'deps'),
      path.join(reactNativeWasmDir, 'src', 'ReactCommon'),
      path.join(reactNativeDir, 'ReactCommon'),
      ...[
        'react/renderer/graphics/platform/cxx',
        'react/nativemodule/core',
        'runtimeexecutor',
        'jsi',
        'jsiexecutor',
        'callinvoker',
        'yoga',
        'reactperflogger',
        'react/nativemodule/samples',
      ].map((file) => path.join(reactNativeReactCommonDir, file)),
    ],
    sources: [
      ...getFilesFromWasmReactCommonDir('react/renderer/textlayoutmanager'),

      ...getFilesFromReactCommonDir('react/nativemodule/core/ReactCommon'),
      ...getFilesFromReactCommonDir('react/renderer/components/view'),
      ...getFilesFromReactCommonDir('react/renderer/components/text'),
      ...getFilesFromReactCommonDir('react/renderer/components/root'),
      ...getFilesFromReactCommonDir('react/renderer/attributedstring'),
      ...getFilesFromReactCommonDir('react/renderer/uimanager'),
      ...getFilesFromReactCommonDir('react/renderer/textlayoutmanager'),
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
      ...getFilesFromReactCommonDir('jsiexecutor/jsireact'),
      ...getFilesFromReactCommonDir('yoga/yoga'),
      ...getFilesFromReactCommonDir('yoga/yoga/event'),
      ...getFilesFromReactCommonDir('yoga/yoga/internal'),
      ...[
        'react/bridging/LongLivedObject.cpp',
        'react/nativemodule/samples/ReactCommon/SampleTurboCxxModule.cpp',
        'reactperflogger/reactperflogger/BridgeNativeModulePerfLogger.cpp',
        'logger/react_native_log.cpp',
      ].map((file) => path.join(reactNativeReactCommonDir, file)),
      ...[
        'MethodCall.cpp',
        'Instance.cpp',
        'CxxNativeModule.cpp',
        'ModuleRegistry.cpp',
        'NativeToJsBridge.cpp',
        'RAMBundleRegistry.cpp',
        'JSExecutor.cpp',
        'JSBigString.cpp',
        'ReactMarker.cpp',
      ].map((file) => path.join(reactNativeCxxReactPath, file)),
    ],
  };
};

const getReactNativeWasm = (reactNativeWasmDir, reactNativeDir) => {
  const getFilesFromReactNativeWasmDir = getFilesFromRootDir(path.join(reactNativeWasmDir, 'src'));
  const reactNativeReactCommonDir = path.join(reactNativeDir, 'ReactCommon');

  return {
    name: 'app',
    compilerFlags: ['-pthread', '-s', 'USE_SDL=2', '-fexceptions'],
    includes: [
      '/usr/local/opt/glog/include',
      '/usr/local/opt/boost/include',
      '/usr/local/opt/gflags/include',
      '/usr/local/opt/fmt/include',
      '/usr/local/opt/double-conversion/include',
      path.join(reactNativeWasmDir, 'deps'),
      path.join(reactNativeWasmDir, 'src', 'ReactCommon'),
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
    definitions: [],
    sources: [
      ...getFilesFromReactNativeWasmDir('Libraries/ReactNativeWasm/UIManager'),
      ...getFilesFromReactNativeWasmDir('Libraries/ReactNativeWasm/Animated'),
      ...getFilesFromReactNativeWasmDir('Libraries/Components/View'),
      ...getFilesFromReactNativeWasmDir('Libraries/Components/Text'),
      ...getFilesFromReactNativeWasmDir('Libraries/ReactNativeWasm/Config'),
      ...getFilesFromReactNativeWasmDir('Libraries/Utilities/Timing'),
      ...getFilesFromReactNativeWasmDir('Libraries/Utilities/DeviceInfo'),
      ...getFilesFromReactNativeWasmDir('Libraries/Utilities/DevSettings'),
      ...getFilesFromReactNativeWasmDir('Libraries/Utilities/SourceCode'),
      ...getFilesFromReactNativeWasmDir('Libraries/Utilities/StatusBarManager'),
      ...getFilesFromReactNativeWasmDir('Libraries/Utilities'),
      ...getFilesFromReactNativeWasmDir('Libraries/ReactNativeWasm/Renderer/SDL'),
      ...[
        'ReactWasmEntry.cpp',
        'Libraries/ReactNativeWasm/Scheduler/SchedulerDelegate.cpp',
        'Libraries/ReactNativeWasm/NativeQueue/NativeQueue.cpp',
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
      [
        getDoubleConversion(reactNativeWasmDir),
        getFmt(reactNativeWasmDir),
        getFolly(reactNativeWasmDir),
        getReactNative(reactNativeWasmDir, reactNativeDir),
        getReactNativeWasm(reactNativeWasmDir, reactNativeDir),
      ],
      'includes',
      [path.join(reactNativeWasmDir, 'src', 'stubs')],
    ),
    'definitions',
  );

  return {
    libraries,
  };
};
