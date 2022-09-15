mergeInto(LibraryManager.library, {
  insertScriptTag: function (scriptUrl) {
    const script = document.createElement('script');

    script.src = UTF8ToString(scriptUrl);

    document.body.appendChild(script);
  },
  setGlobalVariable: function (name, value) {
    window[UTF8ToString(name)] = UTF8ToString(value);

    window['__turboModuleProxy'] = (name) => {
      const result = Module.__turboModuleProxy(name);

      console.warn('__turboModuleProxy', name, result);

      if (result) {
        const wrappedModule = {
          ...result,
          getConstants: () => JSON.parse(result.getConstants()),
        };

        return wrappedModule;
      }

      if (name === 'SourceCode') {
        console.error('JS SourceCode Implementation needs to be removed');
        return {
          getConstants: () => ({ scriptURL: 'react-native.bundle.js' }),
        };
      }

      if (name === 'DeviceInfo') {
        console.error('JS DeviceInfo Implementation needs to be removed');
        return {
          Dimensions: {
            window: {},
            screen: {},
          },
        };
      }

      return null;
    };
  },
});
