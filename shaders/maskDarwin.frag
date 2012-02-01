#version 120

varying vec2 textCoord;

uniform sampler2D camTexture;

// Read
uniform sampler2D pingColorTexture;
uniform sampler2D pingTimeTexture;

void main() {
    
    vec4 colorCam = texture2D(camTexture, textCoord).bgra;
    vec4 colorPing = texture2D(pingColorTexture, textCoord);
    
    float lumCam = dot(colorCam.rgb, vec3(0.33));
    
    float seuil = 0.2;
        
    if (lumCam >= seuil) {
        gl_FragData[0].r = colorCam.r;
        gl_FragData[0].g = colorCam.g;
        gl_FragData[0].b = colorCam.b;
        gl_FragData[0].a = 1. / (0.2) * (lumCam - seuil);
    }
    
    
    
    
    if (colorPing.a > gl_FragData[0].a) {
        gl_FragData[0] = colorPing;
    }
    
    
    gl_FragData[1] = vec4(0.);
}
