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
#define far 30.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5

// Data would normally be read from files


 GLuint program;
	Model *m;
	 Model *m2;


// vertex array object
	unsigned int bunnyVertexArrayObjID;

void init(void)
{
	// vertex buffer object, used for uploading the geometry
	unsigned int bunnyVertexBufferObjID;
	unsigned int bunnyIndexBufferObjID;
	unsigned int bunnyNormalBufferObjID;	
	unsigned int bunnyTexCoordBufferObjID;
	
	GLuint myTex;
	// Reference to shader program

	dumpInfo();

	// GL inits
	glClearColor(0.1,0.7,0.3,0);
	glEnable(GL_DEPTH_TEST);
	printError("GL inits");
	
	m = LoadModelPlus("bunnyplus.obj");	
	m2 = LoadModelPlus("teapot.obj");

	// Load and compile shader
	program = loadShaders("lab2-5.vert", "lab2-5.frag");
	printError("init shader");
	
	printError("init arrays");
}

void UploadObjectMatrices(float x, float y, float z, float ry, mat4 view, mat4 projection, GLuint prg)
{
	mat4 transMatrix = T(x, y, z);
   mat4 rotMatrix = Ry(ry);
   mat4 total = Mult(rotMatrix, transMatrix);
   total = Mult(view, total);
   total = Mult(projection, total);
   
   mat3 normalMatrix = mat4tomat3(transMatrix);
    
    glUniformMatrix3fv(glGetUniformLocation(prg, "normalMatrix"), 1, GL_TRUE, normalMatrix.m);
   glUniformMatrix4fv(glGetUniformLocation(prg, "totMatrix"), 1, GL_TRUE, total.m);
}


void display(void)
{
	printError("pre display");
	
	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	
    mat4 projection = frustum(left, right, bottom, top, near, far);
    
    vec3 p = SetVector(cos(t/720) * 4.0f, 1.0f, sin(t/720) * 4.0f);
    vec3 l = SetVector(0.0, 0.0, 0.0);
    vec3 v = SetVector(0, 1.0, 0);
    mat4 view = lookAtv(p, l, v);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    UploadObjectMatrices(1.0f, 0.0f, 0.0f, 0.1f, view, projection, program);       
   DrawModel(m, program, "in_Position", "in_Normal", "inTexCoord");
   	
   	UploadObjectMatrices(-1.0f, 0.0f, 0.0f, 0.1f, view, projection, program);    
   DrawModel(m, program, "in_Position", "in_Normal", "inTexCoord");
   
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
	glutCreateWindow ("GL3 white triangle example");
	glutDisplayFunc(display); 
	glutTimerFunc(20, &OnTimer, 0);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	init ();
	glutMainLoop();
}
