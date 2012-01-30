#version 150

in vec2 vertPosition;
in vec2 textPosition;

smooth out vec2 textCoord;

void main() {
    gl_Position = vec4(vertPosition, 0, 1);
    textCoord = textPosition;
}
