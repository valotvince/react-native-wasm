#!/usr/bin/env node

const path = require('path');

const build = require('./build');
const serve = require('./serve');
const deps = require('./deps');

const appDir = process.cwd();
const reactNativeWasmDir = path.resolve(__dirname, '..');
const reactNativeDir = path.resolve(path.join(appDir, 'node_modules', 'react-native'));

const argv = process.argv.slice(2);

const start = async () => {
  const options = { appDir, reactNativeWasmDir, reactNativeDir, buildConfig: { debug: !!argv['--debug'] } };

  await deps(options);
  await build(options);

  serve(options);
};

start();
