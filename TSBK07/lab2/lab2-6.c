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
	
	m = LoadModel("bunnyplus.obj");	
	


	// Load and compile shader
	program = loadShaders("lab2-6.vert", "lab2-6.frag");
	printError("init shader");
	
	// Upload geometry to the GPU:
	
	// Allocate and activate Vertex Array Object
	
	 glGenVertexArrays(1, &bunnyVertexArrayObjID);
    glGenBuffers(1, &bunnyVertexBufferObjID);
    glGenBuffers(1, &bunnyIndexBufferObjID);
    glGenBuffers(1, &bunnyNormalBufferObjID); 
    glGenBuffers(1, &bunnyTexCoordBufferObjID); 
    
    LoadTGATextureSimple("maskros512.tga", &myTex);
    printf("mytex : %d", myTex);
    glBindTexture(GL_TEXTURE_2D, myTex);
	 glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0
  
  
    glBindVertexArray(bunnyVertexArrayObjID);

    // VBO for vertex data
    glBindBuffer(GL_ARRAY_BUFFER, bunnyVertexBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->vertexArray, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
    glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

    // VBO for normal data
    glBindBuffer(GL_ARRAY_BUFFER, bunnyNormalBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->normalArray, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "in_Normal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(program, "in_Normal"));
    
     if (m->texCoordArray != NULL)
    {
        glBindBuffer(GL_ARRAY_BUFFER, bunnyTexCoordBufferObjID);
        glBufferData(GL_ARRAY_BUFFER, m->numVertices*2*sizeof(GLfloat), m->texCoordArray, GL_STATIC_DRAW);
        glVertexAttribPointer(glGetAttribLocation(program, "inTexCoord"), 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(glGetAttribLocation(program, "inTexCoord"));
    }
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bunnyIndexBufferObjID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->numIndices*sizeof(GLuint), m->indexArray, GL_STATIC_DRAW);

   
	
	// End of upload of geometry
	
	printError("init arrays");
}


void display(void)
{
	printError("pre display");
	
	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
    mat4 transMatrix = T(0.0f, 0.0f, 0.0f);
    mat4 rotMatrix = Ry(0.1f);
    mat4 projection = frustum(left, right, bottom, top, near, far);
    
    vec3 p = SetVector(.0f, 1.0f, -1.5f);
    vec3 l = SetVector(0.0, 0.0, 0.0);
    vec3 v = SetVector(0, 1.0, 0);
    mat4 view = lookAtv(p, l, v);
    
    mat4 total = Mult(rotMatrix, transMatrix);
    total = Mult(view, total);
    total = Mult(projection, total);
    
    mat3 normalMatrix = mat4tomat3(transMatrix);
    
    
   glUniformMatrix3fv(glGetUniformLocation(program, "normalMatrix"), 1, GL_TRUE, normalMatrix.m);
   //glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
   //glUniformMatrix4fv(glGetUniformLocation(program, "transMatrix"), 1, GL_TRUE, transMatrix.m);
   glUniformMatrix4fv(glGetUniformLocation(program, "totMatrix"), 1, GL_TRUE, total.m);
   //glUniformMatrix4fv(glGetUniformLocation(program, "rotMatrix3"), 1, GL_TRUE, rotMatrix3);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// clear the screen
	
	glBindVertexArray(bunnyVertexArrayObjID);    // Select VAO
   glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);

	
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
	glutCreateWindow ("Garaud");
	glutDisplayFunc(display); 
	glutTimerFunc(20, &OnTimer, 0);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	init ();
	glutMainLoop();
}
