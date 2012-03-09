#version 120

varying vec2 textureCoord;

uniform sampler2D textureCam;
uniform sampler2D textureMask;

void main() {    
    vec4 colorCam = texture2D(textureCam, textureCoord).bgra;
    vec4 colorMask = texture2D(textureMask, textureCoord);
    
    gl_FragColor = colorCam;
    
    gl_FragColor.r = (colorCam.r * (1. - colorMask.a) + colorMask.r * colorMask.a);
    gl_FragColor.g = (colorCam.g * (1. - colorMask.a) + colorMask.g * colorMask.a);
    gl_FragColor.b = (colorCam.b * (1. - colorMask.a) + colorMask.b * colorMask.a);
    gl_FragColor.a = 1.;
}
