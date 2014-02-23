#version 150

in  vec3 in_Position;
in vec3 in_Normal;
in  vec3 in_Color;
in vec2 inTexCoord;


uniform mat4 totMatrix;
uniform mat3 normalMatrix;

out vec3 fragColor;
out vec2 fragTexCoord;
out vec3 fragNormal;
out float fragDiffuse;

const vec3 light = vec3(0.58, 0.58, 0.58);
const float diffuseIntensity = 1.0;


void main(void)
{
	fragTexCoord = inTexCoord;
	fragNormal = normalMatrix * in_Normal;
	float diffuse = diffuseIntensity * max(dot(normalize(fragNormal), light), 0.0);
	fragColor = vec3(diffuse, diffuse, diffuse);
	gl_Position = totMatrix * vec4(in_Position, 1.0);
}
