const createHostObjectProxy = (logPrefix, hostObjectProxy) =>
  new Proxy(hostObjectProxy, {
    get: function (target, propName) {
      if (target[propName]) {
        // console.log(`${logPrefix}[get][${propName}]`, 'PropName already exists');

        const nativeResult = target[propName];

        // console.log(`${logPrefix}[get][${propName}]`, 'Native result', { nativeResult });

        return nativeResult;
      }

      if (propName === 'getConstants') {
        return (...args) => {
          const nativeResult = target.runFunction(target, propName, args);

          // console.log(`${logPrefix}[get][${propName}]`, 'Native result', { nativeResult });

          return nativeResult;
        };
      }

      const nativeResult = target.get(target, propName);

      // console.log(`${logPrefix}[get][${propName}]`, 'Native result', { nativeResult });

      return nativeResult;
    },
    set: function (target, propName, propValue) {
      // console.log(`${logPrefix}[set][${propName}]`, propValue);

      target[propName] = propValue;
    },
  });
