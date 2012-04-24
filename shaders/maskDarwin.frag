#version 120

varying vec2 textureCoord;

uniform sampler2D textureCam;
uniform sampler2D textureMask0;
uniform sampler2D textureMask1;
uniform sampler2D textureMask2;
uniform sampler2D textureMask3;
uniform sampler2D textureMask4;
uniform sampler2D textureMask5;
//uniform sampler2D textureMask6;
//uniform sampler2D textureMask7;

uniform sampler2D timingMask0;
uniform sampler2D timingMask1;

uniform float threshold;
uniform float time;

void main() {
    
    vec4 colorCam = texture2D(textureCam, textureCoord);
	gl_FragData[0] = texture2D(textureMask0, textureCoord);
	gl_FragData[1] = texture2D(textureMask1, textureCoord);
	gl_FragData[2] = texture2D(textureMask2, textureCoord);
	gl_FragData[3] = texture2D(textureMask3, textureCoord);
	gl_FragData[4] = texture2D(textureMask4, textureCoord);
	gl_FragData[5] = texture2D(textureMask5, textureCoord);
	//gl_FragData[6] = texture2D(textureMask6, textureCoord);
	//gl_FragData[7] = texture2D(textureMask7, textureCoord);
	int nbMask = 6;
	
    vec4 timerMask0 = texture2D(timingMask0, textureCoord);
	vec4 timerMask1 = texture2D(timingMask1, textureCoord);
	
	float timerMask[6] = float[6](timerMask0.r, timerMask0.g, timerMask0.b, timerMask0.a, timerMask1.r, timerMask1.g/*, timerMask1.b/*, timerMask1.a*/); 
    
    float PI = 3.14159265358979323846264;
    float zero = PI / (threshold - 1);
	int i = 0;
	

    float luma = dot(colorCam.rgb, vec3(0.33));
    
	for(i = 0; i < nbMask - 1 ; ++i) {
		if(gl_FragData[i].a > 0) {
			timerMask[i] += 0.005;
		} else {
			if (luma >= threshold) {
				gl_FragData[i].a = pow((cos(luma * zero - zero) / 2 + 0.5), 4);
				//gl_FragData[i].a = 1. / (1. - threshold) * (luma - threshold); 
				gl_FragData[i].rgb = colorCam.rgb;
				timerMask[i] = 0.005;
				i = nbMask;
			}
		}
	}

	if(i == (nbMask - 1)) {
		//float alphaCam = (1. / (1. - threshold) * (luma - threshold));
		if (luma >= threshold) {
			gl_FragData[i].a = pow((cos(luma * zero - zero) / 2 + 0.5), 4);
			//gl_FragData[i].a = (1. / (1. - threshold) * (luma - threshold));
			gl_FragData[i].rgb = colorCam.rgb;
			timerMask[i] = 0.005;
		} else {
			timerMask[i] += 0.005;
		}
	}

	if(timerMask[0] >= time) {
		gl_FragData[0] = vec4(0.,0.,0.,0.);
		timerMask[0] = 0;
		
		for(i = 1; i < nbMask; i++) {
			if(gl_FragData[i].a > 0) {
				gl_FragData[i - 1] = gl_FragData[i];
				gl_FragData[i] = vec4(0.,0.,0.,0.);
				timerMask[i - 1] = timerMask[i];
				timerMask[i] = 0; 
			} else {
				i = nbMask;
			}
		}
	}

	gl_FragData[nbMask].r = timerMask[0];
	gl_FragData[nbMask].g = timerMask[1];
	gl_FragData[nbMask].b = timerMask[2];
	gl_FragData[nbMask].a = timerMask[3];
	gl_FragData[nbMask + 1 ].r = timerMask[4];
	gl_FragData[nbMask + 1 ].g = timerMask[5];
	//gl_FragData[nbMask + 1 ].b = timerMask[6];
	//gl_FragData[nbMask + 1 ].a = timerMask[7];
}