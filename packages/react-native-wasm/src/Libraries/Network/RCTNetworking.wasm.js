/**
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 *
 * @flow strict-local
 * @format
 */

'use strict';

const RCTNetworking = {
  addListener(eventType, listener, context) {
    console.warn('RCTNetworking.addListener called but no implemented');
  },

  sendRequest() {
    console.warn('RCTNetworking.sendRequest called but no implemented');
  },

  abortRequest() {
    console.warn('RCTNetworking.abortRequest called but no implemented');
  },

  clearCookies() {
    console.warn('RCTNetworking.clearCookies called but no implemented');
  },
};

export default RCTNetworking;
