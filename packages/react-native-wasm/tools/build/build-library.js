const fs = require('fs/promises');
const fsSync = require('fs');
const path = require('path');
const { generateChecksum, generateFileChecksum } = require('../checksum');

const spawnPromise = require('../spawn-promise');

const getChecksums = async (files) => {
  const checksums = await Promise.all(
    files.map(async (file) => ({ file, checksum: await generateFileChecksum(file) })),
  );

  return checksums.reduce((accumulator, { checksum, file }) => {
    accumulator[file] = checksum;

    return accumulator;
  }, {});
};

const buildManifest = async (library) => {
  return {
    ...library,
    sources: await getChecksums(library.sources),
  };
};

const getManifest = async (outputDirectory) => {
  try {
    return JSON.parse(await fs.readFile(path.join(outputDirectory, 'manifest.json')));
  } catch {
    return null;
  }
};

const shouldRebuild = (manifest, existingManifest) => {
  return generateChecksum(JSON.stringify(manifest)) !== generateChecksum(JSON.stringify(existingManifest));
};

const getObjectOutputFilePath = (outputDirectory, sourcePath) => {
  return path.join(outputDirectory, sourcePath.substring(sourcePath.lastIndexOf('/') + 1).replace(/\.(cpp|cc)/, '.o'));
};

module.exports = async (cwd, library, { debug }) => {
  const outputDirectory = path.join(cwd, 'build', debug ? 'debug' : 'release', library.name);

  await fs.mkdir(path.join(outputDirectory), { recursive: true });

  const manifest = await buildManifest(library);
  const existingManifest = await getManifest(outputDirectory);

  if (!shouldRebuild(manifest, existingManifest)) {
    return;
  }

  const sources = library.sources.reduce((accumulator, source) => {
    if (
      !existingManifest ||
      manifest.sources[source] !== existingManifest.sources[source] ||
      !fsSync.existsSync(getObjectOutputFilePath(outputDirectory, source))
    ) {
      accumulator.push(source);
    }

    return accumulator;
  }, []);

  const options = ['USE_PTHREADS=1'].map((warning) => `-s${warning}`);

  if (debug) {
    options.push('-g', '-O0', '-fexceptions', '-fsanitize=address');
  } else {
    options.push('-O2');
  }

  await spawnPromise(
    'emcc',
    [
      '-std=c++17',
      '-Wall',

      ...options,

      ...library.compilerFlags,
      ...library.definitions.map((definition) => `-D${definition}`),
      ...library.includes.map((include) => `-I${include}`),
      ...sources,

      '-c',
    ],
    { cwd: outputDirectory },
  );

  await spawnPromise(
    'emar',
    ['rcs', `${library.name}.a`, ...library.sources.map((file) => getObjectOutputFilePath(outputDirectory, file))],
    {
      cwd: outputDirectory,
    },
  );

  const updatedSources = library.sources.reduce((accumulator, source) => {
    if (fsSync.existsSync(getObjectOutputFilePath(outputDirectory, source))) {
      accumulator.push(source);
    }

    return accumulator;
  }, []);

  await fs.writeFile(
    path.join(outputDirectory, 'manifest.json'),
    JSON.stringify(
      {
        ...manifest,
        sources: await getChecksums(updatedSources),
      },
      null,
      2,
    ),
  );
};
