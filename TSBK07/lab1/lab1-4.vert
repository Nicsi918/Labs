#version 150

in  vec3 in_Position;
in  vec3 in_Color;
uniform mat4 myMatrix;

out vec3 fragColor;


void main(void)
{
	fragColor = in_Color;
	gl_Position = myMatrix * vec4(in_Position, 1.0);
}
