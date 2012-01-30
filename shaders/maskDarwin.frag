#version 120

varying vec2 textCoord;
//out vec4 OutColor;
uniform sampler2D camTexture;

void main() {
    gl_FragColor = texture2D(camTexture, textCoord).bgra;
}
