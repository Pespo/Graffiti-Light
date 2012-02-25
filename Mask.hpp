#ifndef __MASK_H__
#define __MASK_H__

#include "Config.hpp"
#include "Texture.hpp"
#include <iostream>

using namespace std;

struct Mask {
    Mask() { cout << "Mask : new" << endl; color = new Texture(GL_LINEAR, GL_LINEAR); timer = new Texture(GL_NEAREST, GL_NEAREST); };
    Mask(Mask& copy) : color(copy.color), timer(copy.timer) {};
    ~Mask() {};
    
    Texture* color;
    Texture* timer;
};

struct PingPong {
    PingPong() { cout << "PingPong : new" << endl; in = new Mask(); out = new Mask(); };
    Mask* in;
    Mask* out;
    void swap() {
        if (LM_DEBUG) cout << "PingPong : swap" << endl;
        Mask temp = *in;
        *in = *out;
        *out = temp;
    }
};

#endif // __MASK_H__