import React, { useCallback, useEffect, useRef, useState } from 'react';
import { AppRegistry, View, Text, StyleSheet } from 'react-native';
import { Button } from './src/components/button';

const Demo = () => {
  const intervalId = useRef();
  const [timer, setTimer] = useState(0);

  const startTimer = useCallback(() => {
    intervalId.current = setInterval(() => {
      setTimer((previousTimer) => previousTimer + 1);
    }, 2000);
  }, []);

  const stopTimer = useCallback(() => {
    clearInterval(intervalId.current);
    intervalId.current = null;
  }, []);

  useEffect(() => {
    // Hacky trick to remove once I figured out re-render & button press management
    const listener = (event: KeyboardEvent) => {
      if (event.key === 'Enter') {
        if (intervalId.current) {
          stopTimer();
        } else {
          startTimer();
        }
      }
    };

    document.addEventListener('keydown', listener);

    return () => {
      document.removeEventListener('keydown', listener);
      clearInterval(intervalId.current);
    };
  }, []);

  return (
    <View style={styles.root}>
      <View style={styles.appContainer}>
        <Text style={styles.title}>Hello world!</Text>
        <Text style={styles.timer}>Timer: {timer}</Text>
        <View style={styles.boxesContainer}>
          <View style={[styles.box, { backgroundColor: 'rgba(255, 0, 0, 0.1)' }]}></View>
          <View style={[styles.box, { backgroundColor: 'rgba(0, 255, 0, 0.1)' }]}></View>
          <View style={[styles.box, { backgroundColor: 'rgba(0, 0, 255, 0.1)' }]}></View>
        </View>
        <View style={styles.buttonsContainer}>
          <Button onPress={startTimer} text="Start Timer" />
          <Button onPress={stopTimer} text="Stop Timer" />
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
  buttonsContainer: {
    marginTop: 10,
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
