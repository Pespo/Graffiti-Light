// simple toon fragment shader
// www.lighthouse3d.com

uniform sampler2D texture; 

varying vec3 normal, lightDir;

void main()
{
	vec2 coord=vec2(10.0/255.0, 10.0/255.0);
    gl_Fragment=texture2D(texture, coord);
}
