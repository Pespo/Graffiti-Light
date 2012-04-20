#version 120

#extension GL_EXT_gpu_shader4 : enable

varying vec2 textureCoord;

uniform sampler2D textureCam;
uniform sampler2D textureMask;
uniform sampler2D timingMask;

uniform float threshold;

void main() {
    
    vec4 colorCam = texture2D(textureCam, textureCoord);
    vec4 colorMask = texture2D(textureMask, textureCoord);
    vec4 timerMask = texture2D(timingMask, textureCoord);
    
    float PI = 3.14159265358979323846264;
    float zero = PI / (threshold - 1);
    
    // float luma = colorCam.r * 0.3 + colorCam.g * 0.59 + colorCam.b * 0.11;
    float luma = dot(colorCam.rgb, vec3(0.33));
    
    if (luma >= threshold) {
        gl_FragData[0].r = colorCam.r;
        gl_FragData[0].g = colorCam.g;
        gl_FragData[0].b = colorCam.b;
        
        // gl_FragData[0].a = pow((cos(luma * zero - zero) / 2 + 0.5), 4);
        
        gl_FragData[0].a = 1. / (1. - threshold) * (luma - threshold);
    }
    
    if (colorMask.a > gl_FragData[0].a) {
        gl_FragData[0] = colorMask;
        gl_FragData[1].r = timerMask.r + 0.03;
    }
    
    if (gl_FragData[1].r > 0.9) {
        gl_FragData[0].a = 0.;
        gl_FragData[1].r = 0;
    }
}