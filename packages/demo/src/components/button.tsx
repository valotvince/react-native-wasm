import React from 'react';
import { Text, StyleSheet, TouchableOpacity } from 'react-native';
import { withNavigation } from '../navigation/navigation';

interface ButtonProps {
  focus: boolean;
  text: string;
  onPress: () => void;
}

const ButtonComponent = ({ focus, text, onPress }: ButtonProps) => {
  return (
    <TouchableOpacity style={[styles.wrapper, focus ? styles.focus : {}]} onPress={onPress}>
      <Text style={styles.text}>{text}</Text>
    </TouchableOpacity>
  );
};

export const Button = withNavigation(ButtonComponent);

const styles = StyleSheet.create({
  wrapper: {
    height: 40,
    paddingVertical: 5,
    paddingHorizontal: 10,
    backgroundColor: 'rgba(0, 0, 0, 0.1)',
    marginTop: 5,
  },
  focus: {
    borderColor: 'red',
    borderWidth: 2,
  },
  text: {
    fontSize: 15,
    color: 'white',
  },
});
