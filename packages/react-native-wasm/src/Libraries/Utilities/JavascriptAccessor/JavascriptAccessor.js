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

      console.log('Debug: fbBatchedBridge', decodedMethodName, ...decodedArgs);

      const result = window.__fbBatchedBridge[decodedMethodName].call(window.__fbBatchedBridge, ...decodedArgs);

      console.log('Debug: fbBatchedBridge result:', result);

      return allocateUTF8(JSON.stringify(result));
    } catch (error) {
      console.error(error);
    }
  },
  setGlobalVariableFunction__proxy: 'sync',
  setGlobalVariableFunction: function (name) {
    const decodedName = UTF8ToString(name);

    console.log('setGlobalVariableFunction', decodedName);

    console.log(Module.WasmRuntime)

    window[decodedName] = (...args) => window.WasmRuntime.invoke(decodedName, args);

    // console.log('setGlobalVariableFunction', window[decodedName])
  },
  setGlobalVariableObject__proxy: 'sync',
  setGlobalVariableObject: function (name, objectPointer) {
    console.log('Debug: setGlobalVariableObject', UTF8ToString(name), objectPointer);

    // window[UTF8ToString(name)] = {};
  },
  setGlobalVariable__proxy: 'sync',
  setGlobalVariable: function (name, value) {
    const decodedName = UTF8ToString(name);
    const decodedValue = UTF8ToString(value);

    try {
      window[decodedName] = JSON.parse(decodedValue);
    } catch {
      window[decodedName] = decodedValue
    }
  },
});
