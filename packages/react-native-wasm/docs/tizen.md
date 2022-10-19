# TIZEN

## About

It was both tested on 6.5 Emulator and a real device with a optimized build (both wasm & react-native js bundle).
For now, the demo app works on the 6.5 Device but not on the 6.5 Emulator for an unknown reason.

## react-native js bundle

- `Libraries/Core/setUpNavigator.js` is trying to set the navigator's product property to React Native value which doen't work and make the whole js stop
