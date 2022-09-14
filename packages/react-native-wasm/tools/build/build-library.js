const fs = require('fs/promises');
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

module.exports = async (cwd, library) => {
  const outputDirectory = path.join(cwd, 'build', library.name);

  await fs.mkdir(path.join(outputDirectory), { recursive: true });

  const manifest = await buildManifest(library);
  const existingManifest = await getManifest(outputDirectory);

  if (!shouldRebuild(manifest, existingManifest)) {
    return;
  }

  const sources = library.sources.reduce((accumulator, source) => {
    if (existingManifest && manifest.sources[source] !== existingManifest.sources[source]) {
      accumulator.push(source);
    }

    return accumulator;
  }, []);

  await spawnPromise(
    'emcc',
    [
      '-std=c++17',

      ...library.compilerFlags,
      ...library.definitions.map((definition) => `-D${definition}`),
      ...library.includes.map((include) => `-I${include}`),
      ...sources,

      '-c',
    ],
    { cwd: outputDirectory },
  );

  const files = await fs.readdir(outputDirectory);

  await spawnPromise('emar', ['rc', `${library.name}.a`, ...files.filter((file) => file.endsWith('.o'))], {
    cwd: outputDirectory,
  });

  await fs.writeFile(path.join(outputDirectory, 'manifest.json'), JSON.stringify(manifest));
};
