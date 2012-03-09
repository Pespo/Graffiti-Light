#version 120

varying vec2 textureCoord;

uniform sampler2D textureCam;
uniform sampler2D textureMask;
uniform sampler2D timingMask;

uniform float threshold;

void main() {
    
    vec4 colorCam = texture2D(textureCam, textureCoord);
    vec4 colorMask = texture2D(textureMask, textureCoord);
    float PI = 3.14159265358979323846264;
    float zero = PI / (threshold - 1);
    
    float luma = colorCam.r * 0.3 + colorCam.g * 0.59 + colorCam.b * 0.11;
    
    if (luma >= threshold) {
/*      gl_FragData[0].r = colorCam.r;
        gl_FragData[0].g = colorCam.g;
        gl_FragData[0].b = colorCam.b;*/

        // For testing : the more luminance the more black it is.
        gl_FragData[0].rgb = vec3(1 - luma);

        // ==========================
        // =     Equation tests     =
        // Examples with threshold = 0.5
        // ==========================
        
        // http://fooplot.com/index.php?q0=(cos(x*(pi/(0.8-1))-(pi/(0.8-1)))/2+0.5)%5e4
        gl_FragData[0].a = pow((cos(luma * zero - zero) / 2 + 0.5), 4);
        
        // http://fooplot.com/index.php?q0=1/(1-0.5)*(x-0.5)%3b
//        gl_FragData[0].a = 1 / (1 - threshold) * (luma - threshold);
    }
    
    if (colorMask.a > gl_FragData[0].a) {
        gl_FragData[0] = colorMask;
    }
    
    gl_FragData[1] = vec4(0.);
}