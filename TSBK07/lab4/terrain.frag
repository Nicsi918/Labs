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
	
	vec3 ref = reflect(-light, normalizedNormal);
	float specular = dot(ref, v);
	float temp = max(0.0, specular);
	specular = temp * 1.0 * pow(specular, 20);
	
	/*
	if (specular > 0.0)
	{
		specular = 1.0 * pow(specular, 20);
	}
	specular = max(specular, 0.0);*/
	
	float tidal = sin(time / 720) * 0.05;
	//float conc = step(0.0, exPos.y) - smoothstep(0.05, 0.1, exPos.y);
	float water = step(0.0, exPos.y) - smoothstep(0.05 + tidal, 0.1 + tidal, exPos.y);
	float grass = smoothstep(0.05 + tidal, 0.1 + tidal, exPos.y) - smoothstep(5.0, 6.0, exPos.y);
	float dirt = smoothstep(5.0, 6.0, exPos.y);
	
	//vec4 concC =  conc * texture(tex1, texCoord);
	vec4 waterC = water * vec4(0.0, 0.0, 0.5, 1.0);
	vec4 grassC =  grass * texture(tex2, texCoord);
	vec4 dirtC = dirt * texture(tex3, texCoord);
	
	//outColor = diffuse * (concC + grassC + dirtC);
	outColor = diffuse * (waterC + grassC + dirtC) + specular * waterC;
	//outColor = diffuse * (conc * texture(tex1, texCoord) + grass * texture(tex2, texCoord) + dirt * texture(tex3, texCoord)); 
	//outColor = vec4(exNormal, 1.0);
}
