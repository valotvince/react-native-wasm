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
        <Text style={styles.timer}>Timer: {timer}</Text>
        <View style={styles.boxesContainer}>
          <View style={[styles.box, { backgroundColor: 'red' }]}></View>
          <View style={[styles.box, { backgroundColor: 'green' }]}></View>
          <View style={[styles.box, { backgroundColor: 'blue' }]}></View>
        </View>
        <View style={styles.boxesContainer}>
          <View style={[styles.box, { backgroundColor: 'rgba(255, 0, 0, 0.1)' }]}></View>
          <View style={[styles.box, { backgroundColor: 'rgba(0, 255, 0, 0.1)' }]}></View>
          <View style={[styles.box, { backgroundColor: 'rgba(0, 0, 255, 0.1)' }]}></View>
        </View>
      </View>
    </View>
  );
};

const styles = StyleSheet.create({
  root: {
    padding: 10,
    width: 300,
    height: 300,
  },
  appContainer: {
    padding: 10,
    backgroundColor: 'white',
    borderWidth: 2,
    borderColor: 'red',
    flex: 1,
    flexDirection: 'column',
  },
  boxesContainer: {
    flexDirection: 'row',
    height: 85,
    marginTop: 10,
  },
  box: {
    flex: 1,
  },
  title: {
    color: 'blue',
    fontSize: 20,
  },
  timer: {
    color: 'red',
    fontSize: 15,
  },
});

AppRegistry.registerComponent('main', () => Demo);
