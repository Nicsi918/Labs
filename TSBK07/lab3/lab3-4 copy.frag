#version 150

in vec3 fragColor;
in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragPos;

out vec4 out_Color;

const float diffuseIntensity = 1.0;
const vec3 light = vec3(0.58, 0.58, 0.58);

uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent[4];
uniform bool isDirectional[4];

uniform sampler2D texUnit;
uniform mat4 totMatrix;
uniform mat3 normalMatrix;

void main(void)
{	
	vec3 normalizedNormal = normalize(fragNormal);
	vec3 view = normalize(-fragPos);
	
	//int i;
	//for (i = 0; i < 4; i++)
	//{
		//vec3 light;
		//if (isDirectional[i])
		//{
	vec3  light = normalMatrix * lightSourcesDirPosArr[2];
		//}

		
		//Diffuse
	float diffuse = max(0.0, dot(normalizedNormal, light));
	
		//Specular
	vec3 ref = reflect(-light, normalizedNormal);
	float specular = dot(ref, view);
	if (specular > 0.0)
	{
		specular = 1.0 * pow(specular, specularExponent[2]);
	}
	specular = max(specular, 0.0);
	
		//Together
	float shade = 0.7 * diffuse + 1.0 * specular;
	out_Color = vec4(shade * lightSourcesColorArr[2], 1.0);
	//}
}
