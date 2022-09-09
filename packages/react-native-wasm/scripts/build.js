#!/usr/bin/env node

const { spawn } = require('child_process');
const path = require('path');
const overrideRn = require('./override-rn');

const spawnPromise = (...args) =>
  new Promise((resolve, reject) => {
    const child = spawn(...args);

    child.on('exit', () => {
      resolve();
    });

    child.stdout.on('data', (data) => {
      console.log(`${data}`);
    });

    child.stderr.on('data', (data) => {
      console.error(`${data}`);
    });
  });

(async () => {
  // await overrideRn();

  await spawnPromise(
    'emcc',
    [
      '-lembind',
      '-s',
      'USE_SDL=2',
      '-std=c++14',
      'src/Libraries/Components/Text/Text.cpp',
      'src/Libraries/Component/component.cpp',
      'src/Libraries/ReactNativeWasm/reactNativeWasm.cpp',
      'src/entry.cpp',
      '-o',
      'dist/entry.html',
    ],
    { cwd: path.join(__dirname, '..') },
  );

  // await spawnPromise('cp', [
  //   '-r',
  //   path.resolve('./node_modules/react-native/Libraries'),
  //   path.join(__dirname, '..', 'Libraries'),
  // ]);

  await spawnPromise('react-native', ['bundle', '--platform', 'wasm', '--entry-file', './index.tsx']);
})();
