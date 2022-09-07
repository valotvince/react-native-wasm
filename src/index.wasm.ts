import Text from './Libraries/Components/Text/Text';

export default {
  // Components
  get Text(): Text {
    return require('./Libraries/Components/Text/Text').default;
  },
};
