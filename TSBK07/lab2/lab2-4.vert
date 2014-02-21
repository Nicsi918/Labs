#version 150

in  vec3 in_Position;
in vec3 in_Normal;
in  vec3 in_Color;
in vec2 inTexCoord;


uniform mat4 projectionMatrix;
uniform mat4 transMatrix;
uniform mat4 totMatrix;

out vec3 fragColor;
out vec2 fragTexCoord;


void main(void)
{
	fragTexCoord = inTexCoord;
	fragColor = vec3(inTexCoord, 0.0);
	//fragColor = vec3(inTexCoord.x + in_Normal.x, inTexCoord.y + in_Normal.y, in_Normal.z);
	//fragColor = vec3(sin(in_Normal.x), cos(in_Normal.y), sin(in_Normal.z));
	gl_Position = projectionMatrix * totMatrix * vec4(in_Position, 1.0);
}
