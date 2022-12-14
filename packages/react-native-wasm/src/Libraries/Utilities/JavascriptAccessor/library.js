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
      script.onerror = () => {
        console.error('An error happened during the loading of the react-native bundle');
      }

      document.body.appendChild(script);
  },

  setGlobalVariableObject__proxy: 'sync',
  setGlobalVariableObject: function(name) {
    const decodedName = UTF8ToString(name);

    const logPrefix = `[object][${decodedName}]`;

    console.log(logPrefix, 'Registering object');

    const nativeObject = window[`native__${decodedName}`];

    window[decodedName] = createHostObjectProxy(logPrefix, nativeObject);
  },

  registerObjectFunction__proxy: 'sync',
  registerObjectFunction: function (targetHandle, utf8Name) {
    const name = UTF8ToString(utf8Name);

    const target = Emval.toValue(targetHandle);

    console.log(`[registerObjectFunction]`, target, name);

    target[name] = (...args) => {

      const nativeResult = target.runFunction(target, name, args);

      // TODO Memory leak: Find a way to release handles when React clones/remove a component
      // if (nativeResult) {
      //   console.log(`[registerObjectFunction]`, {name, args, address: nativeResult.$$.ptr.toString(16), smartAddress: nativeResult.$$.smartPtr.toString(16)});
      // }

      return nativeResult;
    }
  },

  setGlobalVariableFunction__proxy: 'sync',
  setGlobalVariableFunction: function (name) {
    const decodedName = UTF8ToString(name);

    const logPrefix = `[function][${decodedName}]`;

    console.log(logPrefix, decodedName);

    window[decodedName] = (...args) => {
      // console.log(logPrefix, 'Called with', args);

      const result = window.WasmRuntime.invoke(decodedName, args);

      // console.log(logPrefix, 'result', result);

      if (decodedName === '__turboModuleProxy') {
        if (!result) {
          return result;
        }

        return createHostObjectProxy(logPrefix, result);
      }

      return result;
    }
  }
});
