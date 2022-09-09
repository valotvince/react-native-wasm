#ifndef COMPONENT_H
#define COMPONENT_H

#include <iostream>
#include <string>

class Component {
    public:
        Component(std::string name, float width, float height);

    private:
        std::string name;
        float width, height;
};

#endif
