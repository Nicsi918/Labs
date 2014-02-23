#version 150

in vec3 fragColor;
in vec2 fragTexCoord;
out vec4 out_Color;

uniform sampler2D texUnit;


void main(void)
{	
	//out_Color = vec4(fragColor, 0.0);
	out_Color = texture(texUnit, fragTexCoord);	
}
