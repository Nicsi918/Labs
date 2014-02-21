#version 150

in vec3 fragColor;
in vec2 fragTexCoord;
out vec4 out_Color;
in vec4 gl_FragCoord;

uniform sampler2D texUnit;
uniform int fadeMod;

void main(void)
{	
	int x = int(gl_FragCoord.x);
	int y = int(gl_FragCoord.y);
	
	float fx = mod(x, fadeMod) * 100;
	float fy = mod(y, fadeMod) * 100;
	float fz = fx * fy;
	
	float tempX = texture(texUnit, fragTexCoord).x;
	float tempY = texture(texUnit, fragTexCoord).y;
	float tempZ = texture(texUnit, fragTexCoord).z;
	
	out_Color = vec4(min(fx, tempX), min(fy, tempY), min(fz, tempZ), 0.0);

	
	//out_Color = vec4(mod(x, fadeMod), mod(y,fadeMod), 0.0, 0.0);
	//out_Color = texture(texUnit, fragTexCoord);	
}
