#version 120

varying vec2 textCoord;
//out vec4 OutColor;
uniform sampler2D camTexture;
uniform sampler2D maskTexture;

void main() {
    
    vec4 colorMask = texture2D(maskTexture, textCoord);
    vec4 colorCam = texture2D(camTexture, textCoord).bgra;
    
    gl_FragColor.r = (colorCam.r * (1. - colorMask.a) + colorMask.r * colorMask.a);
    gl_FragColor.g = (colorCam.g * (1. - colorMask.a) + colorMask.g * colorMask.a);
    gl_FragColor.b = (colorCam.b * (1. - colorMask.a) + colorMask.b * colorMask.a);
    gl_FragColor.a = 1.;
    
//    if (colorMask.a < 0.5) {
//        gl_FragColor = texture2D(maskTexture, textCoord);
//    } else {
//        gl_FragColor = texture2D(camTexture, textCoord).bgra;
//    }
}
