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

uniform sampler2D texUnit1;
uniform sampler2D texUnit2;

uniform mat4 viewMatrix;
uniform mat3 normalMatrix;

void main(void)
{	
	vec3 normalizedNormal = normalize(fragNormal);
	vec3 v = normalize(-fragPos);
	vec3 diffuseLighting = vec3(0,0,0);
	vec3 specularLighting = vec3(0,0,0);

	
	for (int i = 0; i < 4; i++)
	{
		vec3 light;
		if (isDirectional[i])
		{
			light = normalMatrix * lightSourcesDirPosArr[i];
		}else
		{
			light = normalize(vec3(viewMatrix * vec4(lightSourcesDirPosArr[i], 1.0)) - fragPos);
		}
		//Diffuse
		float diffuse = max(0.0, dot(normalizedNormal, light));
	
		//Specular
		vec3 ref = reflect(-light, normalizedNormal);
		float specular = dot(ref, v);
		if (specular > 0.0)
		{
			specular = 1.0 * pow(specular, specularExponent[i]);
		}
		specular = max(specular, 0.0);
		
		diffuseLighting = diffuseLighting + diffuse * lightSourcesColorArr[i];
		specularLighting = specularLighting + specular * lightSourcesColorArr[i];
	}
	
	out_Color = vec4(diffuseLighting * vec3(0.2 * texture(texUnit1, fragTexCoord) + 0.8 * texture(texUnit2, fragTexCoord)) + specularLighting, 1.0);
	
}
