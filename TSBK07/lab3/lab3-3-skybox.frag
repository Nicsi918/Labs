#version 150

in vec2 fragTexCoord;
in vec3 fragNormal;

out vec4 out_Color;

uniform sampler2D texUnit;



void main(void)
{	
	out_Color = texture(texUnit, fragTexCoord);	
	//out_Color = vec4(1.0, 0.0, 0.0, 1.0);
}

