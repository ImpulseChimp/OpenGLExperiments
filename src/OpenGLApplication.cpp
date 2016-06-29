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
void processKeyboardInput(unsigned char key, int x, int y);
void processMouseInput(int button, int state, int x, int y);
void toggleFullScreen();
void processInput();
void updateGameState();
int getCurrentTime();

enum VAO_IDs { Triangles, VAOSize };
enum Buffer_IDs { ArrayBuffer, BufferSize };
enum Attrib_IDs { vPosition = 0, vOffset = 6 };

enum Game_Loop { MS_PER_UPDATE = 16 };

GLuint vertexArrays[VAOSize];
GLuint buffers[BufferSize];
const GLuint VerticesCount = 6;
bool isFullscreen = false;

float aspectRatio = 16.0/9.0;
int windowHeight = 400;
int windowWidth = windowHeight * aspectRatio;


int main(int argc, char **argv) {

	initializeWindow(argc, argv, false);
	initializeDependencies();

	bool runGame = true;
	double previous = getCurrentTime();
	double lag = 0.0;

	while (runGame)
	{
		double current = getCurrentTime();
		double elapsed = current - previous;
		previous = current;
		lag += elapsed;

		//processInput();

		while (lag >= MS_PER_UPDATE)
		{
			updateGameState();
			lag -= MS_PER_UPDATE;
		}

		renderScene();
	}

	return 1;
}

int getCurrentTime()
{
	return glutGet(GLUT_ELAPSED_TIME);
}

void processInput()
{

}

void updateGameState()
{

}

void initializeWindow(int argc, char **argv, bool fullScreen) {
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
	glutKeyboardFunc(processKeyboardInput);
	glutMouseFunc(processMouseInput);
	//glutIdleFunc(renderScene);
	//glutReshapeFunc(changeSize);
}

void initializeDependencies(void) {

	glGenVertexArrays(VAOSize, vertexArrays);
	glBindVertexArray(vertexArrays[Triangles]);

	GLfloat vertices[VerticesCount + 1][2] = {
		{ -0.90, -0.90 },
		{  0.85, -0.90 },
		{ -0.90,  0.85 },
		{  0.90, -0.85 },
		{  0.90,  0.90 },
		{ -0.85,  0.90 },
		{  3.00,  3.00 }
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

	glVertexAttribPointer(vOffset, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(VerticesCount));
	glEnableVertexAttribArray(vOffset);

	glClearColor(0, 1, 0, 1);
	//glPointSize(1.0);
	glutMainLoop();
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

	glColor3f(1.0f, 1.0f, 0.0f);
	glBindVertexArray(vertexArrays[Triangles]);
	glDrawArrays(GL_TRIANGLES, 0, VerticesCount);
	cerr << "RENDERED" << endl;

	glFlush();
}

void processKeyboardInput(unsigned char key, int x, int y) {
	switch(key) {
	case 'w':

		break;
	case 'a':

		break;
	case 's':

		break;
	case 'd':

		break;
	case 'f':
		toggleFullScreen();
		break;
	case 27:
		exit(0);
		break;
	}
}

void processMouseInput(int button, int state, int x, int y) {

}

void toggleFullScreen() {
	if (!isFullscreen)
	{
		glutFullScreen();
		isFullscreen = true;
	}
	else {
		glutReshapeWindow(windowWidth, windowHeight);
		isFullscreen = false;
	}
}
