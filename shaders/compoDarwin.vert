#version 120

attribute vec2 vertexPos;
attribute vec2 texturePos;

varying vec2 textureCoord;

void main() {
    gl_Position = vec4(vertexPos, 0, 1);
    textureCoord = texturePos;
}
