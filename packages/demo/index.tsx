import React, { useEffect, useState } from 'react';
import { AppRegistry, View, Text, StyleSheet } from 'react-native';

const Demo = () => {
  const [timer, setTimer] = useState(0);

  useEffect(() => {
    const id = setInterval(() => {
      setTimer((previousTimer) => previousTimer + 1);
    }, 5000);

    return () => clearInterval(id);
  }, []);

  return (
    <View style={styles.root}>
      <View style={styles.appContainer}>
        <Text style={styles.title}>Hello world!</Text>
        <Text>Timer: {timer}</Text>
      </View>
    </View>
  );
};

const styles = StyleSheet.create({
  root: {
    padding: 10,
    width: '300',
    height: '300',
  },
  appContainer: {
    padding: 10,
    backgroundColor: 'white',
    borderWidth: 2,
    borderColor: 'red',
  },
  title: {
    fontSize: 20,
  },
});

AppRegistry.registerComponent('main', () => Demo);
