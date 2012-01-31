#version 120

attribute vec2 vertPosition;
attribute vec2 textPosition;

varying vec2 textCoord;

void main() {
    gl_Position = vec4(vertPosition, 0, 1);
    textCoord = textPosition;
    textCoord.y = -textCoord.y;
}
