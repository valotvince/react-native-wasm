import React, { useCallback, useMemo, useRef, useState } from 'react';
import { AppRegistry, View, Text, StyleSheet } from 'react-native';
import { Button } from './src/components/button';
import { NavigationProvider } from './src/navigation/navigation';

const Demo = () => {
  const intervalId = useRef();
  const [timer, setTimer] = useState(0);

  const stopTimer = useCallback(() => {
    clearInterval(intervalId.current);
    intervalId.current = null;
  }, []);

  const startTimer = useCallback(() => {
    stopTimer();

    intervalId.current = setInterval(() => {
      setTimer((previousTimer) => previousTimer + 1);
    }, 2000);
  }, [stopTimer]);

  const startTimerNav = useMemo(() => ({ id: 'start-timer', down: 'stop-timer', enter: startTimer }), [startTimer]);
  const stopTimerNav = useMemo(() => ({ id: 'stop-timer', up: 'start-timer', enter: stopTimer }), [stopTimer]);

  return (
    <NavigationProvider defaultFocus="start-timer">
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
            <Button navigation={startTimerNav} text="Start Timer" />
            <Button navigation={stopTimerNav} text="Stop Timer" />
          </View>
        </View>
      </View>
    </NavigationProvider>
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
