// Lab 4, terrain generation

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"

mat4 projectionMatrix;





vec3 getPoint(GLfloat *vertexArray, int width, int x, int z)
{
	int i = 3 * (x + z * width);
	
	vec3 retV = SetVector(vertexArray[i], vertexArray[i + 1], vertexArray[i + 2]);

	return retV;
}

vec3 calcTriangleNormal(GLfloat *vertexArray, int width, int x1, int z1, int x2, int z2, int x3, int z3)
{
	vec3 a = getPoint(vertexArray, width, x1, z1);
	vec3 b = getPoint(vertexArray, width, x2, z2); 
	vec3 c = getPoint(vertexArray, width, x3, z3);
	
	vec3 ab = VectorSub(b, a);
	vec3 ac = VectorSub(c, a);
	
	vec3 retV = Normalize(CrossProduct(ab, ac));
	//vec3 retV = CrossProduct(ab, ac);
	
	
	/*printf("Vertex 1 : %4.2f %4.2f %4.2f\n", a.x, a.y, a.z);
	printf("Vertex 2 : %4.2f %4.2f %4.2f\n", b.x, b.y, b.z);
	printf("Vertex 3 : %4.2f %4.2f %4.2f\n", c.x, c.y, c.z);
	printf("Vertices %d %d, %d %d, %d %d gives : \n", x1, z1, x2, z2, x3, z3);
	printf("Normal : %4.2f, %4.2f, %4.2f\n", retV.x, retV.y, retV.z); */
	return retV;
}

vec3 getPointFromHeightMap(GLfloat *vertexArray, int width, float x, float z)
{	

	//printf("in %4.2f, %4.2f \n", x, z);	
	float xa, xd, za, zd, zo;
	
	xa = (int)x;
	za = (int)z;
	xd = (int)x + 1;
	zd = (int)z + 1;
	
	float distA = sqrt((xa - x) * (xa - x) + (za - z) * (za - z));
	float distD = sqrt((xd - x) * (xd - x) + (zd - z) * (zd - z));
	
	vec3 normal;
	//printf("Distance A : %4.2f, Distance D : %4.2f\n", distA, distD);
	if (distA < distD)
	{
		normal = calcTriangleNormal(vertexArray, width, xa, za, xa, za + 1, xa + 1, za);
	}else
	{
		normal = calcTriangleNormal(vertexArray, width, xa + 1, za, xa, za + 1, xa + 1, za + 1);
	}
	//printf("Normal : %4.2f %4.2f %4.2f\n", normal.x, normal.y, normal.z);
	
	int i = 3 * ((int)x + (int)z * width);
	
	//printf("Point : %4.2f %4.2f %4.2f\n", xa, vertexArray[i+1], za);
	
	float planeA = normal.x;
	float planeB = normal.y;
	float planeC = normal.z;
	float planeD = -planeA * xa - planeB * vertexArray[i+1] - planeC * za;
	
	//printf("Plane : %4.2f %4.2f %4.2f, %4.2f\n", planeA, planeB, planeC, planeD);
	
	float y = (- planeD - planeC * z - planeA * x) / planeB;
	
	vec3 retV = SetVector(x, y, z);
	
	//printf("RetV : %4.2f %4.2f %4.2f\n", retV.x, retV.y, retV.z);
	
	return retV;
	
}

Model* GenerateTerrain(TextureData *tex)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	int x, z;
	
	GLfloat *vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);
	
	printf("bpp %d\n", tex->bpp);
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)*3 + 0] = x;
			vertexArray[(x + z * tex->width)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / 25.0;
			vertexArray[(x + z * tex->width)*3 + 2] = z;
// Normal vectors. You need to calculate these.
			normalArray[(x + z * tex->width)*3 + 0] = 0.0;
			normalArray[(x + z * tex->width)*3 + 1] = 0.0;
			normalArray[(x + z * tex->width)*3 + 2] = 0.0;
// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)*2 + 0] = x; // (float)x / tex->width;
			texCoordArray[(x + z * tex->width)*2 + 1] = z; // (float)z / tex->height;
		}
	for (x = 0; x < tex->width-1; x++)
		for (z = 0; z < tex->height-1; z++)
		{
		// Triangle 1
			indexArray[(x + z * (tex->width-1))*6 + 0] = x + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 1] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 2] = x+1 + z * tex->width;
		// Triangle 2
			indexArray[(x + z * (tex->width-1))*6 + 3] = x+1 + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 4] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 5] = x+1 + (z+1) * tex->width;
		}
	
	// End of terrain generation
	
	for (x = 1; x < tex->width - 1; x++)
		for (z = 1; z < tex->height-1; z++)
			{
				vec3 n1 = calcTriangleNormal(vertexArray, tex->width, x, z-1, x-1, z, x, z);
				vec3 n2 = calcTriangleNormal(vertexArray, tex->width, x-1, z, x-1, z+1, x, z);
				vec3 n3 = calcTriangleNormal(vertexArray, tex->width, x, z, x-1, z+1, x, z+1);
				vec3 n4 = calcTriangleNormal(vertexArray, tex->width, x, z, x, z+1, x+1, z);
				vec3 n5 = calcTriangleNormal(vertexArray, tex->width, x+1, z-1, x, z, x+1, z);
				vec3 n6 = calcTriangleNormal(vertexArray, tex->width, x, z-1, x, z, x+1, z-1);
			
				vec3 total = VectorAdd(n1, n2);
				total = VectorAdd(total, n3);
				total = VectorAdd(total, n4);
				total = VectorAdd(total, n5);
				total = VectorAdd(total, n6);
				
				total = Normalize(ScalarMult(total, 1.0f/6.0f));
				
				normalArray[(x + z * tex->width)*3 + 0] = total.x;
				normalArray[(x + z * tex->width)*3 + 1] = total.y;
				normalArray[(x + z * tex->width)*3 + 2] = total.z;
				
				//printf("Final normal : %4.2f, %4.2f, %4.2f\n", total.x, total.y, total.z);
				//printf("X : %4.2f - Y : %4.2f - Z : %4.2f\n", total.x, total.y, total.z);	
			}
	
	// Create Model and upload to GPU:

	Model* model = LoadDataToModel(
			vertexArray,
			normalArray,
			texCoordArray,
			NULL,
			indexArray,
			vertexCount,
			triangleCount*3);

	return model;
}


// vertex array object
Model *m, *m2, *tm;
// Reference to shader program
GLuint program, objectShader;
GLuint tex1, tex2, tex3;
TextureData ttex; // terrain

vec3 p,l,v;

void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 200.0);
	
	p = SetVector(10, 8, 10);
	l = SetVector(20, 8, 20);
	v = SetVector(0, 1, 0);
	

	// Load and compile shader
	program = loadShaders("terrain.vert", "terrain.frag");
	objectShader = loadShaders("objects.vert", "objects.frag");
	glUseProgram(program);
	printError("init shader");
	
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniform1i(glGetUniformLocation(program, "tex1"), 0); // Texture unit 0
	glUniform1i(glGetUniformLocation(program, "tex2"), 1); // Texture unit 0
	glUniform1i(glGetUniformLocation(program, "tex3"), 2); // Texture unit 0
	LoadTGATextureSimple("conc.tga", &tex1);
	LoadTGATextureSimple("grass.tga", &tex2);
	LoadTGATextureSimple("dirt.tga", &tex3);
	
	glActiveTexture(GL_TEXTURE0);
  	glBindTexture(GL_TEXTURE_2D, tex1);
  	glActiveTexture(GL_TEXTURE1);
  	glBindTexture(GL_TEXTURE_2D, tex2);
  	glActiveTexture(GL_TEXTURE2);
  	glBindTexture(GL_TEXTURE_2D, tex3);
	
	// Load terrain data
	LoadTGATextureData("fft-terrain.tga", &ttex);
	
	m = LoadModelPlus("groundsphere.obj");	
	tm = GenerateTerrain(&ttex);
	
	glUseProgram(objectShader);
	glUniformMatrix4fv(glGetUniformLocation(objectShader, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

	printError("init terrain");
}

void checkKeyDowns()
{
	if (keyIsDown('d'))
	{
		vec3 lp = VectorSub(l,p);
		vec3 dir = CrossProduct(v, lp);
		p = VectorSub(p, ScalarMult(Normalize(dir), 0.2));
		l = VectorSub(l, ScalarMult(Normalize(dir), 0.2));
	}
	if (keyIsDown('a'))
	{
		vec3 lp = VectorSub(l,p);
		vec3 dir = CrossProduct(lp, v);
		p = VectorSub(p, ScalarMult(Normalize(dir), 0.2));
		l = VectorSub(l, ScalarMult(Normalize(dir), 0.2));
	}
	if (keyIsDown('s'))
	{
		vec3 lp = VectorSub(l,p);
		p = VectorSub(p, ScalarMult(Normalize(lp), 0.2));
		l = VectorSub(l, ScalarMult(Normalize(lp), 0.2));
	}
	if (keyIsDown('w'))
	{
		vec3 lp = VectorSub(l,p);
		p = VectorAdd(p, ScalarMult(Normalize(lp), 0.2));
		l = VectorAdd(l, ScalarMult(Normalize(lp), 0.2));
	}
	if (keyIsDown('r'))
	{
		vec3 lp = VectorSub(l,p);
		vec3 dir = CrossProduct(v, lp);
		l = VectorSub(l, ScalarMult(Normalize(dir), 0.5));
	}
	if (keyIsDown('e'))
	{
		vec3 lp = VectorSub(l,p);
		vec3 dir = CrossProduct(v, lp);
		l = VectorAdd(l, ScalarMult(Normalize(dir), 0.5));
	}
	if (keyIsDown('o'))
	{
		vec3 v = getPointFromHeightMap(tm->vertexArray, ttex.width, p.x, p.z);
		printf("V : %4.2f, %4.2f, %4.2f\n", v.x, v.y, v.z); 

	}
}

void display(void)
{

	checkKeyDowns();
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	
	mat4 total, modelView, camMatrix;
	
	printError("pre display");
	
	glUseProgram(program);
	
	vec3 h = getPointFromHeightMap(tm->vertexArray, ttex.width, p.x, p.z);
	//p.y = h.y + 5;
	//l.y = h.y + 5;
	
	camMatrix = lookAtv(p,l,v);
	modelView = IdentityMatrix();
	total = Mult(camMatrix, modelView);
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
   glUniform1f(glGetUniformLocation(program, "time"), t); // Texture unit 0
	glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1
	DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");
	
	glUseProgram(objectShader);

	
	int i;
	
	/*for (i = 30; i < 230; i += 10)
	{
		float f = i;
		vec3 v = getPointFromHeightMap(tm->vertexArray, ttex.width, f * 0.763, f * 0.813);
		modelView = T(v.x, v.y, v.z);
		total = Mult(camMatrix, modelView);
		glUniformMatrix4fv(glGetUniformLocation(objectShader, "mdlMatrix"), 1, GL_TRUE, total.m);
		DrawModel(m, objectShader, "inPosition", "inNormal", NULL);
	}*/
	float x1 = sin(t / 720 / 16) * 50 + 100;
	float z1 = cos(t / 720 / 16) * 50 + 100;
	vec3 v = getPointFromHeightMap(tm->vertexArray, ttex.width, x1, z1);
	modelView = T(v.x, v.y, v.z);
	total = Mult(camMatrix, modelView);
	glUniformMatrix4fv(glGetUniformLocation(objectShader, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(m, objectShader, "inPosition", "inNormal", NULL);
	printf("V : %4.2f %4.2f %4.2f\n", v.x, v.y, v.z);
	
	
	
	printError("display 2");	
	glutSwapBuffers();
}

void timer(int i)
{
	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (600, 600);
	glutCreateWindow ("TSBK07 Lab 4");
	glutDisplayFunc(display);
	init ();
	initKeymapManager();
	glutTimerFunc(20, &timer, 0);

	glutMainLoop();
	exit(0);
}
