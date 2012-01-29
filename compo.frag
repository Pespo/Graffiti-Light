#version 150

smooth in vec2 textCoord;
out vec4 OutColor;
uniform sampler2D camTexture;

void main() {
    OutColor = texture(camTexture, textCoord).bgra;
}
