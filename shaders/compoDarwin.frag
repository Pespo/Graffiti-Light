#version 120

varying vec2 textureCoord;

uniform sampler2D textureCam;
uniform sampler2D textureMask1;
uniform sampler2D textureMask2;
uniform sampler2D textureMask3;
uniform sampler2D textureMask4;

void main() {    
    vec4 colorCam = texture2D(textureCam, textureCoord).bgra;
    vec4 colorMask1 = texture2D(textureMask1, textureCoord);
	vec4 colorMask2 = texture2D(textureMask2, textureCoord);
	vec4 colorMask3 = texture2D(textureMask3, textureCoord);
	vec4 colorMask4 = texture2D(textureMask4, textureCoord);

	vec4 max = colorMask1;
	if(max.a < colorMask2.a) max = colorMask2;
	if(max.a < colorMask3.a) max = colorMask3;
	if(max.a < colorMask4.a) max = colorMask4;
    
    gl_FragColor.r = (colorCam.r * (1. - max.a) + max.r * max.a);
    gl_FragColor.g = (colorCam.g * (1. - max.a) + max.g * max.a);
    gl_FragColor.b = (colorCam.b * (1. - max.a) + max.b * max.a);
    gl_FragColor.a = 1.;

	
}