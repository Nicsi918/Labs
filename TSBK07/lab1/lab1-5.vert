#version 150

in  vec3 in_Position;
in  vec3 in_Color;
uniform mat4 rotMatrix1;
uniform mat4 rotMatrix2;
uniform mat4 rotMatrix3;
uniform mat4 transMatrix;


out vec3 fragColor;


void main(void)
{
	fragColor = in_Color;
	gl_Position = transMatrix * rotMatrix1 * rotMatrix2 * rotMatrix3 * vec4(in_Position, 1.0);
}
