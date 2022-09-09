const fs = require('fs');
const path = require('path');

const reactNativePath = path.join(__dirname, 'node_modules', 'react-native');
const reactNativeWasmPath = path.resolve(__dirname, '../react-native-wasm');

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

  const moduleName = path.join(
    originalModuleDir.replace(reactNativeWasmPath, '').replace(reactNativePath, ''),
    originalModuleName,
  );

  if (testExtensions(moduleName)) {
    return moduleName;
  }

  // Local override
  if (testExtensions(path.join(reactNativeWasmPath, 'src', moduleName))) {
    return path.join(reactNativeWasmPath, 'src', moduleName);
  }

  if (testExtensions(path.join(__dirname, 'node_modules', 'react-native', moduleName))) {
    return path.join(__dirname, 'node_modules', 'react-native', moduleName);
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
    return path.join(reactNativeWasmPath, 'src', 'Libraries/Unimplemented');
  }

  return moduleName;
};

module.exports = {
  projectRoot: __dirname,
  resolver: {
    resolveRequest: (context, moduleName, platform) => {
      const redirectedModuleName = getModuleName(context, moduleName);

      console.log({ moduleName, redirectedModuleName });

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
  transformer: {
    getTransformOptions: async () => ({
      transform: {
        experimentalImportSupport: false,
        inlineRequires: true,
      },
    }),
  },
};
