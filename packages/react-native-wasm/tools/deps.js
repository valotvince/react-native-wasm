const path = require('path');
const fs = require('fs/promises');
const spawnPromise = require('./spawn-promise');

const dependencies = [
  {
    name: 'fmt',
    version: '6.2.1',
    remote: 'https://github.com/fmtlib/fmt/archive/refs/tags/6.2.1.tar.gz',
  },
  {
    name: 'boost',
    version: '1.76.0',
    remote: 'https://github.com/boostorg/boost/archive/refs/tags/boost-1.76.0.tar.gz',
  },
  {
    name: 'folly',
    version: '2021.07.22.00',
    remote: 'https://github.com/facebook/folly/releases/download/v2021.07.22.00/folly-v2021.07.22.00.tar.gz',
  },
  {
    name: 'double-conversion',
    version: '1.1.6',
    remote: 'https://github.com/google/double-conversion/archive/refs/tags/v1.1.6.tar.gz',
  },
  {
    name: 'glog',
    version: '0.3.5',
    remote: 'https://github.com/google/glog/archive/refs/tags/v0.3.5.tar.gz',
  },
  {
    name: 'libevent',
    version: '2.1.12',
    remote:
      'https://github.com/libevent/libevent/releases/download/release-2.1.12-stable/libevent-2.1.12-stable.tar.gz',
  },
];

module.exports = async ({ appDir, reactNativeWasmDir, reactNativeDir }) => {
  const depsDir = path.join(reactNativeWasmDir, 'deps');
  const depsArchivesDir = path.join(depsDir, '__archives');

  await fs.mkdir(depsArchivesDir, { recursive: true });

  await Promise.all(
    dependencies.map(async ({ name, version, remote, archiveFormat }) => {
      const archivePath = path.join(depsArchivesDir, `${name}-${version}.tar.gz`);
      const depPath = path.join(depsDir, name);

      try {
        await fs.stat(archivePath);
      } catch {
        await spawnPromise('wget', [remote, '-O', archivePath]);
      }

      try {
        await fs.stat(depPath);
      } catch {
        await fs.mkdir(depPath, { recursive: true });
        await spawnPromise('tar', ['-xf', archivePath, '-C', depPath, '--strip-components', '1']);
      }
    }),
  );
};
