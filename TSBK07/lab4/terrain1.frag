#version 150

out vec4 outColor;

in vec3 exNormal;

in vec3 exPos;
in vec2 texCoord;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
uniform float time;

const float diffuseIntensity = 1.0;
const vec3 light = vec3(0.58, 0.58, 0.58);


void main(void)
{	
	vec3 normalizedNormal = normalize(exNormal);
	vec3 v = normalize(-exPos);	
	outColor = texture(tex1, texCoord);
}
