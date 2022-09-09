const { copyFile } = require('fs/promises');
const path = require('path');

const { overrides } = require('../overrides.json');

const reactNativePath = path.resolve('node_modules/react-native');

module.exports = async () => {
  await Promise.all(
    overrides.map(async ({ file, baseFile }) => {
      // return copyFile(path.resolve(file), path.join(reactNativePath, baseFile));
    }),
  );

  // upgradeOverrides(path.resolve('overrides.json'), { allowConflicts: true });
};
