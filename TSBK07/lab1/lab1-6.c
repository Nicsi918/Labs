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
#include "loadobj.h"

#include <math.h>

// Globals
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
	// Reference to shader program

	dumpInfo();

	// GL inits
	glClearColor(0.1,0.7,0.3,0);
	glEnable(GL_DEPTH_TEST);
	printError("GL inits");
	
	m = LoadModel("bunny.obj");	
	


	// Load and compile shader
	program = loadShaders("lab1-6.vert", "lab1-6.frag");
	printError("init shader");
	
	// Upload geometry to the GPU:
	
	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &bunnyVertexArrayObjID);
    glGenBuffers(1, &bunnyVertexBufferObjID);
    glGenBuffers(1, &bunnyIndexBufferObjID);
    glGenBuffers(1, &bunnyNormalBufferObjID);
    
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
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bunnyIndexBufferObjID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->numIndices*sizeof(GLuint), m->indexArray, GL_STATIC_DRAW);

	
	// End of upload of geometry
	
	printError("init arrays");
}


void display(void)
{
	printError("pre display");
	
	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

	GLfloat rotMatrix1[] = 
	{   cos(t/360), sin(t/360), 0.0f, 0.0f,
 	  -sin(t/360), cos(t/360), 0.0f, 0.0f,
     0.0f, 0.0f, 1.0f, 0.0f,
     0.0f, 0.0f, 0.0f, 1.0f };
   
     
    GLfloat rotMatrix2[] = 
	{   1.0f, 0.0f, 0.0f, 0.0f,
 	  0.0f, cos(t/720), sin(t/720), 0.0f,
     0.0f, -sin(t/720), cos(t/720), 0.0f,
     0.0f, 0.0f, 0.0f, 1.0f }; 
     
     
     GLfloat rotMatrix3[] = 
     {
     cos(t/540), 0.0f, -sin(t/540), 0.0f,
 	  0.0f, 1.0f, 0.0f, 0.0f,
     sin(t/540), 0.0f, cos(t/540), 0.0f,
     0.0f, 0.0f, 0.0f, 1.0f
     };
     
     
     GLfloat transMatrix[] = 
     {
     1.0f, 0.0f, 0.0f, 0.2f,
 	  0.0f, 1.0f, 0.0f, 0.0f,
     0.0f, 0.0f, 1.0f, 0.0f,
     0.0f, 0.0f, 0.0f, 1.0f
     };
     
   //glUniformMatrix4fv(glGetUniformLocation(program, "transMatrix"), 1, GL_TRUE, transMatrix);
   //glUniformMatrix4fv(glGetUniformLocation(program, "rotMatrix1"), 1, GL_TRUE, rotMatrix1);
   //glUniformMatrix4fv(glGetUniformLocation(program, "rotMatrix2"), 1, GL_TRUE, rotMatrix2);
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
	glutCreateWindow ("GL3 white triangle example");
	glutDisplayFunc(display); 
	glutTimerFunc(20, &OnTimer, 0);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	init ();
	glutMainLoop();
}
