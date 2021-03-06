#version 150

in  vec3 in_Position;
in vec3 in_Normal;
in  vec3 in_Color;
in vec2 inTexCoord;

uniform mat4 projectionMatrix;
uniform mat4 totMatrix;
uniform mat4 viewMatrix;
uniform mat4 mdlMatrix;
uniform mat3 normalMatrix;

out vec3 fragColor;
out vec2 fragTexCoord;
out vec3 fragNormal;
out vec3 fragPos;


void main(void)
{
	fragTexCoord = inTexCoord;
	fragNormal = normalMatrix * in_Normal;
	fragColor = vec3(1.0, 1.0, 1.0);
	fragPos = vec3(viewMatrix * mdlMatrix * vec4(in_Position, 1.0));
	gl_Position = projectionMatrix * viewMatrix * mdlMatrix * vec4(in_Position, 1.0);
}
