# react-native-wasm

## DISCLAIMER

This project is a POC not intended to be used on production apps.
It wasn't made by a C++ veteran developer (nor an intermediate one), rather made with remnants of school C++ knowledge, which means it is not optimized at all, full of bugs & memory leaks.

Architecture is not at all optimized, I wanted to go fast and demonstrate it can work on web browsers but also on Smart-TV.

## WHAT

This project intends to add a WASM platform to React-Native, so that it renders an app inside a HTML Canvas.
It uses emcc compiler to compile C++ code to WASM.

## HOW

Clone the project, go to packages/demo and run "yarn start".
You should be able to go to http://localhost:8081 with your browser and see "something" happening

## Thanks

Big thanks to:

- [react-native-windows])(https://github.com/microsoft/react-native-windows) which helped setup the project and provide guidance on where to start and what to do
