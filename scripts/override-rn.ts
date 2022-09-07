const path = require('path');
const { upgradeOverrides } = require('react-native-platform-override');

(async () => {
  upgradeOverrides(path.resolve('overrides.json'), { allowConflicts: true });
})();
