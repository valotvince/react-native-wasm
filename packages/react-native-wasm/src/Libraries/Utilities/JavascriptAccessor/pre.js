const createHostObjectProxy = (logPrefix, hostObjectProxy) =>
  new Proxy(hostObjectProxy, {
    get: function (target, propName) {
      if (propName === 'getConstants') {
        return (...args) => {
          const nativeResult = target.runFunction(propName, args);

          console.log(`${logPrefix}[get][${propName}]`, 'Native result', { nativeResult });

          return nativeResult;
        };
      }

      const nativeResult = target.get(propName);

      console.log(`${logPrefix}[get][${propName}]`, 'Native result', { nativeResult });

      return nativeResult;
    },
    set: function (target, propName, propValue) {
      console.log(`${logPrefix}[set][${propName}]`, propValue);
    },
  });
