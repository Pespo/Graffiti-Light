#version 120

#extension GL_EXT_gpu_shader4 : enable

varying vec2 textureCoord;

uniform sampler2D textureCam;
uniform sampler2D textureMask1;
uniform sampler2D textureMask2;
uniform sampler2D textureMask3;
uniform sampler2D textureMask4;
uniform sampler2D timingMask;

uniform float threshold;
uniform float time;

void main() {
    
    vec4 colorCam = texture2D(textureCam, textureCoord);
    vec4 colorMask1 = texture2D(textureMask1, textureCoord);
	vec4 colorMask2 = texture2D(textureMask2, textureCoord);
	vec4 colorMask3 = texture2D(textureMask3, textureCoord);
	vec4 colorMask4 = texture2D(textureMask4, textureCoord);
    vec4 timerMask = texture2D(timingMask, textureCoord);
    
    float PI = 3.14159265358979323846264;
    float zero = PI / (threshold - 1);
    
    gl_FragData[0] = colorMask1;
	gl_FragData[1] = colorMask2;
	gl_FragData[2] = colorMask3;
	gl_FragData[3] = colorMask4;

	
    float luma = dot(colorCam.rgb, vec3(0.33));
    
	if(gl_FragData[0].a > 0) {
		gl_FragData[4].r = timerMask.r + 0.005;

		if(gl_FragData[1].a > 0) {
			gl_FragData[4].g = timerMask.g + 0.005;

			if(gl_FragData[2].a > 0) {
				gl_FragData[4].b = timerMask.b + 0.005; 

				if (luma >= threshold) {
					gl_FragData[3].a = 1. / (1. - threshold) * (luma - threshold);
					gl_FragData[3].rgb = colorCam.rgb;
					gl_FragData[4].a = timerMask.a + 0.005;
				}
			} else {
				if (luma >= threshold) {
					gl_FragData[2].a = 1. / (1. - threshold) * (luma - threshold);
					gl_FragData[2].rgb = colorCam.rgb;
				}
			}
		} else { 
			if (luma >= threshold) {
				gl_FragData[1].a = 1. / (1. - threshold) * (luma - threshold);
				gl_FragData[1].rgb = colorCam.rgb;
			}
		}
	} else {
		if (luma >= threshold) {
			gl_FragData[0].a = 1. / (1. - threshold) * (luma - threshold); 
			gl_FragData[0].rgb = colorCam.rgb;
		}
	}

	if(gl_FragData[4].r >= time) {
		gl_FragData[0] = vec4(0.,0.,0.,0.);
		gl_FragData[4].r = 0;
		
		if(gl_FragData[1].a > 0) {
			gl_FragData[0] = colorMask2;
			if(gl_FragData[1].a > colorMask2.a) gl_FragData[0] = gl_FragData[1];
			gl_FragData[1] = vec4(0.,0.,0.,0.);
			gl_FragData[4].r = gl_FragData[4].g;
			gl_FragData[4].g = 0;
		
			if(gl_FragData[2].a > 0) {
				gl_FragData[1] = colorMask3;
				if(gl_FragData[2].a > colorMask3.a) gl_FragData[1] = gl_FragData[2];
				gl_FragData[2] = vec4(0.,0.,0.,0.);
				gl_FragData[4].g = gl_FragData[4].b;
				gl_FragData[4].b = 0;
		
				if(gl_FragData[3].a > 0) {
					gl_FragData[2] = colorMask3;
					if(gl_FragData[3].a > colorMask4.a) gl_FragData[2] = gl_FragData[3];
					gl_FragData[3] = vec4(0.,0.,0.,0.);
					gl_FragData[4].b = gl_FragData[4].a;
					gl_FragData[4].a = 0;
				}
			}
		}
	}
}