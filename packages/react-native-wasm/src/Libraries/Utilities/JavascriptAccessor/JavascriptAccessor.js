mergeInto(LibraryManager.library, {
  emscripten_set_timeout: function(cb, msecs, userData) {
    const setTimeout = (window.originalSetTimeout || window.setTimeout);

    {{{ runtimeKeepalivePush() }}}
    return setTimeout(function() {
      {{{ runtimeKeepalivePop() }}}
      callUserCallback(function() {
        {{{ makeDynCall('vi', 'cb') }}}(userData);
      });
    }, msecs);
  },
  emscripten_set_timeout_loop: function(cb, msecs, userData) {
    const setTimeout = (window.originalSetTimeout || window.setTimeout);

    function tick() {
      var t = performance.now();
      var n = t + msecs;
      {{{ runtimeKeepalivePop() }}}
      callUserCallback(function() {
        if ({{{ makeDynCall('idi', 'cb') }}}(t, userData)) {
          // Save a little bit of code space: modern browsers should treat
          // negative setTimeout as timeout of 0
          // (https://stackoverflow.com/questions/8430966/is-calling-settimeout-with-a-negative-delay-ok)
          {{{ runtimeKeepalivePush() }}}
          setTimeout(tick, n - performance.now());
        }
      });
    }

    {{{ runtimeKeepalivePush() }}}

    return setTimeout(tick, 0);
  },

  insertScriptTag__proxy: 'sync',
  insertScriptTag: function (scriptUrl) {
      const script = document.createElement('script');

      script.src = UTF8ToString(scriptUrl);
      script.onload = () => {
        Module.__onBundleLoaded();
      };

      document.body.appendChild(script);
  },

  fbBatchedBridge__proxy: 'sync',
  fbBatchedBridge: async function (methodName, args) {
    try {
      const decodedMethodName = UTF8ToString(methodName);
      const decodedArgs = JSON.parse(UTF8ToString(args));

      console.warn('fbBatchedBridge', window.__fbBatchedBridge[decodedMethodName], ...decodedArgs);

      window.__fbBatchedBridge[decodedMethodName].call(window.__fbBatchedBridge, ...decodedArgs);
    } catch (error) {
      console.error(error);
    }
  },
  setGlobalVariable__proxy: 'sync',
  setGlobalVariable: function (name, value) {
    console.log(typeof window, typeof global, { Module });

    window[UTF8ToString(name)] = UTF8ToString(value);

    window['__turboModuleProxy'] = (name) => {
      const createNativeModuleProxy = (nativeModule) => {
        if (!nativeModule) {
          return null;
        }

        return new Proxy(nativeModule, {
          get: function (target, field) {
            if (field === 'getConstants') {
              return () => JSON.parse(target.getConstants());
            }

            if (field in target) {
              return target[field];
            }

            return (...args) => {
              return target.invoke(field, JSON.stringify(args));
            };
          },
        });
      };

      const result = Module.__turboModuleProxy(name);

      if (result) {
        return createNativeModuleProxy(result);
      }

      if (!result) {
        console.warn(name, 'Implementation needs to be removed');

        return {
          getConstants: () => ({
            scriptURL: 'react-native.bundle.js',
            Dimensions: {
              window: {},
              screen: {},
            },
          }),
          show: () => {},
          hide: () => {},
        };
      }

      return null;
    };
  },
});
