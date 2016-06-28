#define GLEW_STATIC
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GLM/glm.hpp>

#include "vgl.h"
#include "LoadShaders.h"

using namespace std;
using namespace glm;

void initializeWindow(int argc, char **argv, bool fullScreen);
void changeSize(int w, int h);
void renderScene(void);
void initializeDependencies(void);

enum VAO_IDs { Triangles, VAOSize };
enum Buffer_IDs { ArrayBuffer, BufferSize };
enum Attrib_IDs { vPosition = 0 };

GLuint vertexArrays[VAOSize];
GLuint buffers[BufferSize];
const GLuint VerticesCount = 6;

int main(int argc, char **argv) {

	initializeWindow(argc, argv, false);
	initializeDependencies();

	glutMainLoop();
	return 1;
}

void initializeWindow(int argc, char **argv, bool fullScreen) {
	float aspectRatio = 16.0/9.0;
	int windowHeight = 400;
	int windowWidth = windowHeight * aspectRatio;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowPosition(300, 100);
	glutInitWindowSize(windowWidth, windowHeight);
	glClearColor(1, 0, 0, 1);
	glutCreateWindow("Shark Week 2016");

	if (glewInit()) {
		cerr << "Unable to load GLEW" << endl;
		exit(EXIT_FAILURE);
	}

	if (fullScreen)
		glutFullScreen();

	glutInitContextVersion(4, 3);
	glutDisplayFunc(renderScene);
	//glutReshapeFunc(changeSize);
}

void initializeDependencies(void) {

	glGenVertexArrays(VAOSize, vertexArrays);
	glBindVertexArray(vertexArrays[Triangles]);

	GLfloat vertices[VerticesCount][2] = {
		{ -0.90, -0.90 },
		{  0.85, -0.90 },
		{ -0.90,  0.85 },
		{  0.90, -0.85 },
		{  0.90,  0.90 },
		{ -0.85,  0.90 }
	};

	glGenBuffers(BufferSize, buffers);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[ArrayBuffer]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "C:\\Users\\Christopher\\workspace\\OpenGLApplication\\src\\shaders\\vertex\\triangles.vert" },
		{ GL_FRAGMENT_SHADER, "C:\\Users\\Christopher\\workspace\\OpenGLApplication\\src\\shaders\\fragment\\triangles.frag" },
		{ GL_NONE, NULL }
	};

	GLuint program = LoadShaders(shaders);
	glUseProgram(program);

	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(vPosition);

	glClearColor(0, 1, 0, 1);
}

void changeSize(int w, int h) {

	if (h == 0)
		h = 1;

	float ratio =  w * 1.0 / h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45, ratio, 1, 100);

	glMatrixMode(GL_MODELVIEW);
}

void renderScene(void) {

	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(vertexArrays[Triangles]);
	glDrawArrays(GL_TRIANGLES, 0, VerticesCount);

	glFlush();
}

