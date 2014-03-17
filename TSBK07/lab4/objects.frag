#version 150

out vec4 outColor;

in vec3 exNormal;

const float diffuseIntensity = 1.0;
const vec3 light = vec3(0.58, 0.58, 0.58);

void main(void)
{
	//outColor = texture(tex, texCoord);
	
	float diffuse = diffuseIntensity * max(dot(normalize(exNormal), light), 0.0);
	//outColor = diffuse * texture(tex, texCoord);
	//outColor = vec4(exNormal, 1.0);
	outColor = vec4(diffuse, diffuse, diffuse, 1.0);
}
