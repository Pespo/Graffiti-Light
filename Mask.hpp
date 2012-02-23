#ifndef __MASK_H__
#define __MASK_H__

#include "Config.hpp"
#include "Texture.hpp"
#include <iostream>

using namespace std;

struct Mask {
    Mask(int test) : color(GL_LINEAR, GL_LINEAR), timer(GL_NEAREST, GL_NEAREST) { cout << "new Mask" << endl; };
    Mask() { cout << "new Mask" << endl; };
    Mask(Mask& copy) : color(copy.color), timer(copy.timer) {};
    ~Mask() {};
    Texture color;
    Texture timer;
};

struct PingPong {
    PingPong() : in(2), out(2) { cout << "new PingPong" << endl; };
    Mask in;
    Mask out;
    void swap() {        
        Mask tmp = in;
        in = out;
        out = tmp;
    }
};

#endif // __MASK_H__