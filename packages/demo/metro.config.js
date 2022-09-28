const fs = require('fs');
const path = require('path');

const reactNativePath = path.join(__dirname, 'node_modules', 'react-native');
const reactNativeWasmPath = path.resolve(__dirname, '..', 'react-native-wasm');

const reactNativeWasmSourcePath = path.join(reactNativeWasmPath, 'src');
const reactNativeWasmReactNativePath = path.join(reactNativeWasmPath, 'node_modules', 'react-native');

const extensions = ['js', 'jsx', 'ts', 'tsx'];

const testExtensions = (path) => {
  for (extension of extensions) {
    if (fs.existsSync(`${path}.wasm.${extension}`)) {
      return path;
    }

    if (fs.existsSync(`${path}.${extension}`)) {
      return path;
    }
  }
};

const getModuleName = (context, originalModuleName) => {
  const originalModuleDir = path.dirname(context.originModulePath);

  if (!originalModuleName.startsWith('./') && !originalModuleName.startsWith('../')) {
    return originalModuleName;
  }

  const moduleName = path
    .join(originalModuleDir, originalModuleName)
    .replace(`${reactNativeWasmSourcePath}/`, '')
    .replace(`${reactNativePath}/`, '')
    .replace(`${reactNativeWasmReactNativePath}/`, '');

  if (fs.existsSync(moduleName)) {
    return moduleName;
  }

  if (testExtensions(moduleName)) {
    return moduleName;
  }

  const localOverride = path.join(reactNativeWasmSourcePath, moduleName);

  // Local override
  if (testExtensions(localOverride)) {
    return localOverride;
  }

  const reactNativeExisting = path.join(reactNativePath, moduleName);

  if (testExtensions(reactNativeExisting)) {
    return reactNativeExisting;
  }

  if (moduleName.includes('Libraries/Components')) {
    return path.join(
      __dirname,
      'node_modules',
      'react-native',
      'Libraries/Components/UnimplementedViews/UnimplementedView',
    );
  }

  if (moduleName.includes('Libraries')) {
    console.log(moduleName, "isn't implemented");

    return path.join(reactNativeWasmSourcePath, 'Libraries/Unimplemented');
  }

  return moduleName;
};

module.exports = {
  projectRoot: __dirname,
  resolver: {
    resolveRequest: (context, moduleName, platform) => {
      const redirectedModuleName = getModuleName(context, moduleName);

      return context.resolveRequest(context, redirectedModuleName, platform);
    },
    // extraNodeModules: new Proxy(
    //   {},
    //   {
    //     get: (target, name) => path.join(__dirname, `node_modules/${name}`),
    //   },
    // ),
    extraNodeModules: {
      // 'react-native': '/Users/vincentvalot/Projects/react-native-tizen/packages/demo/node_modules/react-native',
      '@valotvince/react-native-wasm': reactNativeWasmPath,
    },
  },
  watchFolders: [reactNativeWasmPath],
  serializer: {
    getModulesRunBeforeMainModule: () => {
      const options = { paths: [process.cwd()] };

      return [require.resolve('react-native/Libraries/Core/InitializeCore', options)];
    },
  },
  transformer: {
    getTransformOptions: async () => ({
      transform: {
        experimentalImportSupport: false,
        inlineRequires: true,
      },
    }),
  },
};
