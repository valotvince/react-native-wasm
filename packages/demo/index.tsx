import React, { useEffect, useState } from 'react';
import { AppRegistry, View, Text } from 'react-native';

const Demo = () => {
  const [timer, setTimer] = useState(0);

  useEffect(() => {
    const id = setInterval(() => {
      setTimer((previousTimer) => previousTimer + 1);
    }, 1000);

    return () => clearInterval(id);
  }, []);

  return (
    <View>
      <Text>Hello world!</Text>
      <Text>Timer: {timer}</Text>
    </View>
  );
};

AppRegistry.registerComponent('main', () => Demo);
