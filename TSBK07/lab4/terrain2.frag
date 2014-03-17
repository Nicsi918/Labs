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
	float diffuse = diffuseIntensity * max(dot(normalize(exNormal), light), 0.0);
	
	//outColor = diffuse * (concC + grassC + dirtC);
	outColor = diffuse * texture(tex1, texCoord);
	//outColor = diffuse * (conc * texture(tex1, texCoord) + grass * texture(tex2, texCoord) + dirt * texture(tex3, texCoord)); 
	//outColor = vec4(exNormal, 1.0);
}
