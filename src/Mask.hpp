#ifndef __MASK_H__
#define __MASK_H__

#include "Config.hpp"
#include "Texture.hpp"
#include <iostream>

using namespace std;

struct Mask {
    Mask() { cout << "Mask : new" << endl; pColor = new Texture(GL_LINEAR, GL_LINEAR); pTimer = new Texture(GL_NEAREST, GL_NEAREST); };
    Mask(Mask& copy) : pColor(copy.pColor), pTimer(copy.pTimer) {};
    ~Mask() {};
    
    Texture* pColor;
    Texture* pTimer;
};

struct PingPong {
    PingPong() { cout << "PingPong : new" << endl; pIn = new Mask(); pOut = new Mask(); };
    Mask* pIn;
    Mask* pOut;
    void swap() {
        if (LM_DEBUG) cout << "PingPong : swap" << endl;
        Mask temp = *pIn;
        *pIn = *pOut;
        *pOut = temp;
    }
};

#endif // __MASK_H__