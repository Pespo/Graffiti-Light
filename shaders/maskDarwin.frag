#version 120

varying vec2 textCoord;
//out vec4 OutColor;
uniform sampler2D camTexture;
//uniform sampler2D pingColorTexture;
//uniform sampler2D pingTimeTexture;
//uniform sampler2D pongColorTexture;
//uniform sampler2D pongTimeTexture;

void main() {
    
    vec4 colorCam = texture2D(camTexture, textCoord).bgra;
    
    float lum = dot(colorCam.rgb, vec3(0.33));
    
    if (lum >= 0.7) {
        gl_FragData[0] = vec4(0., 0., 1., 1.);
    } else {
        gl_FragData[0] = colorCam;
    }
    gl_FragData[1] = vec4(0.);
}
