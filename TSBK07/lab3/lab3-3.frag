#version 150

in vec3 fragColor;
in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragLight;

out vec4 out_Color;

uniform sampler2D texUnit;

const float diffuseIntensity = 1.0;
const vec3 light = vec3(0.58, 0.58, 0.58);


void main(void)
{	
	out_Color =  vec4(0.5, 1.0, 0.5, 0.0);
}