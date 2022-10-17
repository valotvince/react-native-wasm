import React, { createContext, useContext, useEffect, useState } from 'react';
import { DeviceEventEmitter } from 'react-native';

interface NavigationData {
  id: string;
  right?: string;
  left?: string;
  up?: string;
  down?: string;
  enter?: () => void;
}

export interface WithNavigationProps {
  [key: string]: any;
  navigation: NavigationData;
}

export interface WithNavigationDescendingProps {
  [key: string]: any;
  focus: boolean;
}

interface KeyDownEvent {
  key: string;
}

interface NavRef {
  setFocus: (focus: boolean) => void;
  navigation: NavigationData;
}

const navigationHandler = new (class NavigationHandler {
  refs: Record<string, NavRef> = {};
  currentFocus?: string;
  defaultFocus?: string;

  constructor() {
    DeviceEventEmitter.addListener('keydown', this.onKeyDown);
  }

  onKeyDown = (event: KeyDownEvent) => {
    console.log('onKeyDown', event, { refs: this.refs, currentFocus: this.currentFocus });

    if (!this.currentFocus) {
      console.error('No default focus');

      return;
    }

    const currentFocusRef = this.refs[this.currentFocus];

    if (!currentFocusRef) {
      console.error('No ref found with id ', this.currentFocus);

      return;
    }

    let newFocusedId;

    switch (event.key) {
      case 'Up':
        if (!currentFocusRef.navigation.up) {
          return;
        }

        newFocusedId = currentFocusRef.navigation.up;
        break;
      case 'Down':
        if (!currentFocusRef.navigation.down) {
          return;
        }

        newFocusedId = currentFocusRef.navigation.down;
        break;
      case 'Left':
        if (!currentFocusRef.navigation.left) {
          return;
        }

        newFocusedId = currentFocusRef.navigation.left;
        break;
      case 'Right':
        if (!currentFocusRef.navigation.right) {
          return;
        }

        newFocusedId = currentFocusRef.navigation.right;
        break;
      case 'Return':
        if (!currentFocusRef.navigation.enter) {
          return;
        }

        console.log('Return');

        currentFocusRef.navigation.enter();
        break;
    }

    if (!newFocusedId) {
      return;
    }

    const newFocusedRef = this.refs[newFocusedId];

    if (!newFocusedRef) {
      return;
    }

    currentFocusRef.setFocus(false);
    newFocusedRef.setFocus(true);
    this.currentFocus = newFocusedId;
  };

  setDefaultFocus(defaultFocus: string) {
    this.defaultFocus = defaultFocus;
  }

  addRef(ref: NavRef) {
    this.refs[ref.navigation.id] = ref;

    if (!this.currentFocus && ref.navigation.id === this.defaultFocus) {
      this.currentFocus = ref.navigation.id;
      ref.setFocus(true);
    }
  }
})();

const NavigationContext = createContext(navigationHandler);

interface NavigationProviderProps extends React.PropsWithChildren {
  defaultFocus: string;
}

export const NavigationProvider = ({ defaultFocus, children }: NavigationProviderProps) => {
  navigationHandler.setDefaultFocus(defaultFocus);

  return <NavigationContext.Provider value={navigationHandler}>{children}</NavigationContext.Provider>;
};

export const withNavigation =
  (WrappedComponent: React.FunctionComponent<WithNavigationDescendingProps>) => (props: WithNavigationProps) => {
    const NavigationComponent = ({ navigation, ...descendingProps }: WithNavigationProps) => {
      const nav = useContext(NavigationContext);
      const [focus, setFocus] = useState(false);

      useEffect(() => {
        nav.addRef({ setFocus, navigation });
      }, [navigation]);

      return <WrappedComponent {...descendingProps} focus={focus} />;
    };

    return <NavigationComponent {...props} />;
  };
