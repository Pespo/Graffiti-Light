#ifndef __MASK_H__
#define __MASK_H__

#include "Config.hpp"
#include "Texture.hpp"
#include <iostream>

using namespace std;

struct Mask {
    Mask(int sid) : id(sid), color(GL_LINEAR, GL_LINEAR), timer(GL_NEAREST, GL_NEAREST) { cout << "Mask : new " << id << "-" << endl; };
    Mask() { cout << "Mask : new " << endl; };
    Mask(Mask& copy) : id(copy.id), color(copy.color), timer(copy.timer) {};
    ~Mask() {};
    int id;
    Texture color;
    Texture timer;
};

struct PingPong {
    PingPong() { cout << "PingPong : new" << endl; in = new Mask(1); out = new Mask(2); };
    Mask* in;
    Mask* out;
    void swap() {
        cout << "PingPong : swap" << endl;
        cout << "PingPong : in = " << in->id << endl;
        cout << "PingPong : out = " << out->id << endl;
        Mask temp = *in;
        *in = *out;
        *out = temp;
        cout << "PingPong : in = " << in->id << endl;
        cout << "PingPong : out = " << out->id << endl;
    }
};

#endif // __MASK_H__