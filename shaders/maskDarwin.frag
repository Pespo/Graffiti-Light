#version 120

varying vec2 textCoord;

uniform sampler2D camTexture;

// Read
uniform sampler2D pingColorTexture;
uniform sampler2D pingTimeTexture;

// Write
uniform sampler2D pongColorTexture;
uniform sampler2D pongTimeTexture;

void main() {
    
    vec4 colorCam = texture2D(camTexture, textCoord).bgra;
    vec4 colorPing = texture2D(pingColorTexture, textCoord);
    vec4 colorPong = texture2D(pongTimeTexture, textCoord);
    
    float lumCam = dot(colorCam.rgb, vec3(0.33));
    float lumPing = dot(colorPing.rgb, vec3(0.33));
        
    if (lumCam >= 0.9) {
        gl_FragData[0] = vec4(0., 0., 1., 1.);
    } else {
        if (colorPing.b >= 0.8) {
            gl_FragData[0] = colorPing;
        } else {
            gl_FragData[0] = colorCam;
        }
    }
    
    
    gl_FragData[1] = vec4(0.);
}
