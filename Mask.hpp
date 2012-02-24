#ifndef __MASK_H__
#define __MASK_H__

#include "Config.hpp"
#include "Texture.hpp"
#include <iostream>

using namespace std;

struct Mask {
    Mask(int test) : color(GL_LINEAR, GL_LINEAR), timer(GL_NEAREST, GL_NEAREST) { cout << "Mask : new" << endl; };
    Mask() { cout << "Mask : new " << endl; };
    Mask(Mask& copy) : color(copy.color), timer(copy.timer) {};
    ~Mask() {};
    Texture color;
    Texture timer;
};

struct PingPong {
    PingPong() : in(2), out(2) { cout << "PingPong : new" << endl; };
    Mask in;
    Mask out;
    void swap() {
        cout << "PingPong : swap" << endl;
        cout << "PingPong : in = " << &in << endl;
        cout << "PingPong : out = " << &out << endl;
        Mask tmp = in;
        in = out;
        out = tmp;
        cout << "PingPong : in = " << &in << endl;
        cout << "PingPong : out = " << &out << endl;
    }
};

#endif // __MASK_H__