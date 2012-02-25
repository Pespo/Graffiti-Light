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
        cout << "PingPong : swap" << endl;
        cout << "PingPong : in = " << " - " << in->color->getId() << " - " << in->timer->getId() << " - " << endl;
        cout << "PingPong : out = " << " - " << out->color->getId() << " - " << out->timer->getId() << " - " << endl;
        Mask temp = *in;
        *in = *out;
        *out = temp;
        cout << "PingPong : in = " << " - " << in->color->getId() << " - " << in->timer->getId() << " - " << endl;
        cout << "PingPong : out = " << " - " << out->color->getId() << " - " << out->timer->getId() << " - " << endl;
    }
};

#endif // __MASK_H__