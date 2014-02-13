#version 150

in  vec3 in_Position;
in vec3 in_Normal;
in  vec3 in_Color;


out vec3 fragColor;


void main(void)
{
	fragColor = vec3(sin(in_Normal.x), cos(in_Normal.y), sin(in_Normal.z));
	gl_Position = vec4(in_Position, 1.0);
}
