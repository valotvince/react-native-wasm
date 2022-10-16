import React from 'react';
import { Text, StyleSheet, TouchableOpacity } from 'react-native';

interface ButtonProps {
  text: string;
  onPress: () => void;
}

export const Button = ({ text, onPress }: ButtonProps) => {
  return (
    <TouchableOpacity style={styles.wrapper} onPress={onPress}>
      <Text style={styles.text}>{text}</Text>
    </TouchableOpacity>
  );
};

const styles = StyleSheet.create({
  wrapper: {
    height: 40,
    paddingVertical: 5,
    paddingHorizontal: 10,
    backgroundColor: 'rgba(0, 0, 0, 0.1)',
    borderColor: 'red',
    borderWidth: 2,
    marginTop: 5,
  },
  text: {
    fontSize: 15,
    color: 'white',
  },
});
