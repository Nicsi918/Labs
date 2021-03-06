// Lab 1-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

// Should work as is on Linux and Mac. MS Windows needs GLEW or glee.
// See separate Visual Studio version of my demos.
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
#include <stddef.h>
#include <stdio.h>
#include <math.h>

// Globals
#define near 1.0
#define far 1000.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5

#define nrOfMills 1


Point3D lightSourcesColorsArr[] = { {1.0f, 0.0f, 0.0f}, // Red light
                                 {0.0f, 1.0f, 0.0f}, // Green light
                                 {0.0f, 0.0f, 1.0f}, // Blue light
                                 {1.0f, 1.0f, 1.0f} }; // White light

GLfloat specularExponent[] = {10.0, 20.0, 60.0, 5.0};
GLint isDirectional[] = {0,0,1,1};

Point3D lightSourcesDirectionsPositions[] = { {0.0f, 5.0f, 5.0f}, // Red light, positional
                                       {5.0f, 5.0f, 0.0f}, // Green light, positional
                                       {-1.0f, 0.0f, 0.0f}, // Blue light along X
                                       {0.0f, 0.0f, -1.0f} }; // White light along Z

// Data would normally be read from files


GLfloat terrain[3*6] = {1000,0.0,-1000,
						-1000,0.0,-1000,
						-1000,0.0,1000,	
						1000,0.0,-1000,	
						-1000,0.0,1000,	
						1000,0.0,1000};

 GLuint program;
GLuint terrainProgram;
GLuint skyboxProgram;
GLuint skyboxTex;

GLuint tex1;
GLuint tex2;

typedef struct   {
	vec3 pos;
} Windmill;

	Windmill windmills[nrOfMills * nrOfMills];
	
	Model *m;
	 Model *m2;
	Model *m3;
	Model *blade;
	Model *skybox;

	vec3 p;
	vec3 l;
	vec3 v;

	unsigned int vertexArrayObjID;

	
void init(void)
{	
	GLuint myTex;
	// Reference to shader program
	
	unsigned int vertexBufferObjID;

	dumpInfo();

	// GL inits
	glClearColor(0.1,0.2,1.0,0);
	glEnable(GL_DEPTH_TEST);
	printError("GL inits");
	
	m = LoadModelPlus("bunnyplus.obj");	
	skybox = LoadModelPlus("skybox.obj");
	
	int i;
	int j;
	for(j = 0; j < nrOfMills; j++)
	{
		for(i = 0; i  < nrOfMills; i++)
		{
			windmills[j*nrOfMills + i].pos = SetVector(i * -25, 0, -25*j + i * -25);
		}
	}
	
	p = SetVector(5.0f, 10.0f, 0.0f);
   l = SetVector(0.0, 10.0, 0.0);
   v = SetVector(0, 1.0, 0);

	// Load and compile shader
	program = loadShaders("lab3-5.vert", "lab3-5.frag");
	terrainProgram = loadShaders("lab3-3.vert", "lab3-3.frag");
	skyboxProgram = loadShaders("lab3-3-skybox.vert", "lab3-3-skybox.frag");
	
	LoadTGATextureSimple("SkyBox512.tga", &skyboxTex);
	LoadTGATextureSimple("conc.tga", &tex1);
	LoadTGATextureSimple("dirt.tga", &tex2);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, skyboxTex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, tex2);

	glUseProgram(program);
	glUniform1i(glGetUniformLocation(program, "texUnit1"), 1);
	glUniform1i(glGetUniformLocation(program, "texUnit2"), 2);
	
	glUseProgram(skyboxProgram);
	glUniform1i(glGetUniformLocation(skyboxProgram, "texUnit"), 0);

	printError("init shader");
	
	
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);
	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID);
	
	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	
	glBufferData(GL_ARRAY_BUFFER, 6*3*sizeof(GLfloat), terrain, GL_STATIC_DRAW);
	
	glUseProgram(terrainProgram);
	glVertexAttribPointer(glGetAttribLocation(terrainProgram, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(terrainProgram, "in_Position"));
	
	glUseProgram(program);
	glUniform3fv(glGetUniformLocation(program, "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions[0].x);
	glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), 4, &lightSourcesColorsArr[0].x);
	glUniform1fv(glGetUniformLocation(program, "specularExponent"), 4, specularExponent);
	glUniform1iv(glGetUniformLocation(program, "isDirectional"), 4, isDirectional);
	
	printError("init arrays");
}

void UploadObjectMatricesArbRot(float x, float y, float z, vec3 rAxis, float rAngle, mat4 view, mat4 projection, GLuint prg)
{
	mat4 transMatrix = T(x,y,z);
   mat4 invTransMatrix = T(-x,-y,-z);
   mat4 rotMatrix = ArbRotate(rAxis, rAngle);
   mat4 total = Mult(transMatrix, rotMatrix);
   total = Mult(total, invTransMatrix);
   total = Mult(total, transMatrix);
   mat3 normalMatrix = mat4tomat3(total);
   
   glUniformMatrix4fv(glGetUniformLocation(prg, "viewMatrix"), 1, GL_TRUE, view.m);
   glUniformMatrix3fv(glGetUniformLocation(prg, "normalMatrix"), 1, GL_TRUE, normalMatrix.m);
   glUniformMatrix4fv(glGetUniformLocation(prg, "mdlMatrix"), 1, GL_TRUE, total.m);
   glUniformMatrix4fv(glGetUniformLocation(prg, "projectionMatrix"), 1, GL_TRUE, projection.m);

}



void UploadObjectMatrices(float x, float y, float z, float rAngle, mat4 view, mat4 projection, GLuint prg)
{
	mat4 transMatrix = T(x, y, z);
   mat4 rotMatrix = Rx(rAngle);
   mat4 total = Mult(rotMatrix, transMatrix);
   mat3 normalMatrix = mat4tomat3(total);
   
   glUniformMatrix4fv(glGetUniformLocation(prg, "viewMatrix"), 1, GL_TRUE, view.m);
   glUniformMatrix3fv(glGetUniformLocation(prg, "normalMatrix"), 1, GL_TRUE, normalMatrix.m);
   glUniformMatrix4fv(glGetUniformLocation(prg, "mdlMatrix"), 1, GL_TRUE, total.m);
   glUniformMatrix4fv(glGetUniformLocation(prg, "projectionMatrix"), 1, GL_TRUE, projection.m);
}

void UploadObjectMatricesVec(vec3 t, float rAngle, mat4 view, mat4 projection, GLuint prg)
{
	UploadObjectMatrices(t.x, t.y, t.z, rAngle, view, projection, prg);
}

void checkKeyDowns()
{
	if (keyIsDown('d'))
	{
		vec3 lp = VectorSub(l,p);
		vec3 dir = CrossProduct(v, lp);
		p = VectorSub(p, ScalarMult(Normalize(dir), 0.1));
		l = VectorSub(l, ScalarMult(Normalize(dir), 0.1));
	}
	if (keyIsDown('a'))
	{
		vec3 lp = VectorSub(l,p);
		vec3 dir = CrossProduct(lp, v);
		p = VectorSub(p, ScalarMult(Normalize(dir), 0.1));
		l = VectorSub(l, ScalarMult(Normalize(dir), 0.1));
	}
	if (keyIsDown('s'))
	{
		vec3 lp = VectorSub(l,p);
		p = VectorSub(p, ScalarMult(Normalize(lp), 0.1));
		l = VectorSub(l, ScalarMult(Normalize(lp), 0.1));
	}
	if (keyIsDown('w'))
	{
		vec3 lp = VectorSub(l,p);
		p = VectorAdd(p, ScalarMult(Normalize(lp), 0.1));
		l = VectorAdd(l, ScalarMult(Normalize(lp), 0.1));
	}
	if (keyIsDown('r'))
	{
		vec3 lp = VectorSub(l,p);
		vec3 dir = CrossProduct(v, lp);
		l = VectorSub(l, ScalarMult(Normalize(dir), 0.1));
	}
	if (keyIsDown('e'))
	{
		vec3 lp = VectorSub(l,p);
		vec3 dir = CrossProduct(v, lp);
		l = VectorAdd(l, ScalarMult(Normalize(dir), 0.1));
	}
	//printf("X : %4.2f - Y : %4.2f - Z : %4.2f\n", p.x, p.y, p.z);	
}

void drawSkybox(mat4 view, mat4 projection)
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glUseProgram(skyboxProgram);
	//mat4 totMatrix = mat3tomat4(mat4tomat3(view));
	//totMatrix = Mult(projection, totMatrix);
	mat4 transMatrix = T(p.x, p.y - 0.1, p.z);
	mat4 rotMatrix = mat3tomat4(mat4tomat3(view));
	mat4 totMatrix = transMatrix; //Mult(rotMatrix, transMatrix);
	//mat4 totMatrix = Mult(rotMatrix, transMatrix);
	totMatrix = Mult(view, totMatrix);
	totMatrix = Mult(projection, totMatrix);
	glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "totMatrix"), 1, GL_TRUE, totMatrix.m);
	DrawModel(skybox, skyboxProgram, "in_Position", "in_Normal", "inTexCoord");
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void display(void)
{
	checkKeyDowns();
	printError("pre display");
	
	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	
    mat4 projection = frustum(left, right, bottom, top, near, far);
        
    mat4 view = lookAtv(p, l, v);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    vec3 nollV = SetVector(0.0f, 0.0f, 0.0f);
    
    drawSkybox(view, projection);
    
   glUseProgram(program);
   
   	UploadObjectMatricesVec(SetVector(0.0, 10.0, 0.0), 0.0f, view, projection, program);       
  	DrawModel(m, program, "in_Position", "in_Normal", "inTexCoord"); 
    
   glUseProgram(terrainProgram);
   mat4 totMatrix = T(0.0, 0.0, 0.0);
   totMatrix = Mult(view, totMatrix);
   totMatrix = Mult(projection, totMatrix);
   glUniformMatrix4fv(glGetUniformLocation(terrainProgram, "totMatrix"), 1, GL_TRUE, totMatrix.m);

   glBindVertexArray(vertexArrayObjID);	// Select VAO
	glDrawArrays(GL_TRIANGLES, 0, 6*3);	// draw object
	
   //mat4 totMatrix = Mult(view, transMatrix);
   //totMatrix = Mult(projection, totMatrix);
   //glUniformMatrix4fv(glGetUniformLocation(terrainProgram, "totMatrix"), 1, GL_TRUE, totMatrix.m);
   
	printError("display");
	glutSwapBuffers();
}


void OnTimer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("Ter");
	glutDisplayFunc(display); 
	glutTimerFunc(20, &OnTimer, 0);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	initKeymapManager();
	//glutPassiveMotionFunc(&mouseMovement);
	
	init ();
	glutMainLoop();
}
