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
	float diffuse = diffuseIntensity * max(dot(normalize(fragNormal), light), 0.0);
	out_Color = diffuse * vec4(diffuse, diffuse, diffuse, 0.0);
}
