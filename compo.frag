#version 130

smooth in vec2 textCoord;
out vec4 OutColor;

void main() {
    OutColor = vec4(textCoord, 0, 1);
}
