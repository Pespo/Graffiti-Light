#ifndef __MASK_H__
#define __MASK_H__

#include "Config.hpp"
#include "Texture.hpp"
#include <iostream>

using namespace std;

struct Mask {
    Mask() { cout << "Mask : new" << endl;
	pColor0 = new Texture(GL_LINEAR, GL_LINEAR);
	pColor1 = new Texture(GL_LINEAR, GL_LINEAR);
	pColor2 = new Texture(GL_LINEAR, GL_LINEAR);
	pColor3 = new Texture(GL_LINEAR, GL_LINEAR);
	pColor4 = new Texture(GL_LINEAR, GL_LINEAR);
	pColor5 = new Texture(GL_LINEAR, GL_LINEAR);
	//pColor6 = new Texture(GL_LINEAR, GL_LINEAR);
	//pColor7 = new Texture(GL_LINEAR, GL_LINEAR);
	
	pTimer0 = new Texture(GL_NEAREST, GL_NEAREST);
	pTimer1 = new Texture(GL_NEAREST, GL_NEAREST); };
    
	Mask(Mask& copy) : pColor0(copy.pColor0), pColor1(copy.pColor1), pColor2(copy.pColor2), pColor3(copy.pColor3), pColor4(copy.pColor4), pColor5(copy.pColor5),/* pColor6(copy.pColor6), pColor7(copy.pColor7),*/ pTimer0(copy.pTimer0), pTimer1(copy.pTimer1) {};
    ~Mask() {};
    
	Texture* pColor0;
    Texture* pColor1;
	Texture* pColor2;
	Texture* pColor3;
	Texture* pColor4;
	Texture* pColor5;
	//Texture* pColor6;
	//Texture* pColor7;
	
    Texture* pTimer0;
	Texture* pTimer1;
};

struct PingPong {
    
	PingPong() { 
		cout << "PingPong : new" << endl;
		pIn = new Mask();
		pOut = new Mask(); };
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