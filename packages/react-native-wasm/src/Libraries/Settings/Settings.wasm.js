/**
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 *
 * @format
 * @flow
 */

'use strict';

const Settings = {
  get(key: string): mixed {
    console.warn('Settings is not yet supported on WASM');
    return null;
  },

  set(settings: Object) {
    console.warn('Settings is not yet supported on WASM');
  },

  watchKeys(keys: string | Array<string>, callback: Function): number {
    console.warn('Settings is not yet supported on WASM');
    return -1;
  },

  clearWatch(watchId: number) {
    console.warn('Settings is not yet supported on WASM');
  },
};

module.exports = Settings;
