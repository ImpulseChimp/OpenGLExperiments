#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <stdio.h>
#include "player.cpp"
#include "LoadShaders.h"

using namespace std;
using namespace glm;

void initilizeWindow();
void initializeDependencies(void);
void createSquare(float x, float y, float width, float height);
void drawTriangle(float* vertArray);
void initilizePlayer(float x, float y, float height, float width);
void redrawScreen(GLFWwindow* window);
void updateGameState();
int getCurrentTime();
void windowResizeCallback(GLFWwindow* window, int width, int height);
void handleKeyboardInput(GLFWwindow* window, int key, int scancode, int action, int mode);

float* vertices = new float[9];
enum VAO_IDs { Triangles = 0, VAOSize = 1 };
enum Buffer_IDs { ArrayBuffer = 0, VBOSize = 1};
enum Attrib_IDs { vPosition = 0, vOffset = 6 };
enum Game_Loop { MS_PER_UPDATE = 16 };

Player* player;

unsigned int vaoID[VAOSize];
unsigned int vboID[VBOSize];

float xOrigin = 0;
float yOrigin = 0;

GLFWwindow* window;

GLuint vertexArrays[VAOSize];
GLuint buffers[VBOSize];
const GLuint VerticesCount = 6;
bool isFullscreen = false;

float aspectRatio = 16.0f / 9.0f;
float windowHeight = 400;
float windowWidth = windowHeight * aspectRatio;

int main() {

	initilizeWindow();
	initializeDependencies();
	player = new Player(-1, -1, 1, 1);

	bool runGame = true;
	double previous = getCurrentTime();
	double lag = 0.0;
	double current = 0;
	double elapsed = 0;

	int secondCounter = 0;
	int fps = 0;

	while (runGame)
	{
		current = getCurrentTime();
		elapsed = current - previous;
		secondCounter += elapsed;
		previous = current;
		lag += elapsed;

		glfwPollEvents();

		while (lag >= MS_PER_UPDATE)
		{
			updateGameState();
			lag -= MS_PER_UPDATE;
		}

		redrawScreen(window);
		fps++;

		if (secondCounter > 1000)
		{
			secondCounter = 0;
			char title[256];
			title[255] = '\0';
			snprintf(title, 255, "%s %s - [FPS: %d]", "Name", "Version", fps);
			glfwSetWindowTitle(window, title);
			fps = 0;
		}
	}

	glfwTerminate();
	return 0;
}

void initializeDependencies(void) {

	glClearColor(0, 1, 0, 1);

	glGenVertexArrays(VAOSize, &vaoID[Triangles]);
	glBindVertexArray(vaoID[Triangles]);

	glGenBuffers(VBOSize, &vboID[ArrayBuffer]);
	glBindBuffer(GL_ARRAY_BUFFER, vboID[ArrayBuffer]);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "C:\\Users\\Christopher\\Documents\\visual studio 2015\\Projects\\OpenGLApplication\\OpenGLApplication\\triangles.vert" },
		{ GL_FRAGMENT_SHADER, "C:\\Users\\Christopher\\Documents\\visual studio 2015\\Projects\\OpenGLApplication\\OpenGLApplication\\triangles.frag" },
		{ GL_NONE, NULL }
	};

	GLuint program = LoadShaders(shaders);
	glUseProgram(program);
}

void initilizeWindow()
{
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		exit(-1);
	}

	window = glfwCreateWindow(windowWidth, windowHeight, "Shark Week", NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();

	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glfwSetKeyCallback(window, handleKeyboardInput);
	glfwSetWindowSizeCallback(window, windowResizeCallback);
}

void redrawScreen(GLFWwindow* window)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawTriangle(player->getVertexArray());

	glfwSwapBuffers(window);
	glFlush();
}

void updateGameState()
{

}

int getCurrentTime()
{
	return glfwGetTime();
}

//void createSquare(float x, float y, float width, float height)
//{
//	float* vertices = new float[18];  // Vertices for our square
//
//	vertices[0] = x; vertices[1] = y; vertices[2] = 0.0; // Bottom left corner
//	vertices[3] = x; vertices[4] = y + height; vertices[5] = 0.0; // Top left corner
//	vertices[6] = x + width; vertices[7] = y + height; vertices[8] = 0.0; // Top Right corner
//
//	vertices[9] = x; vertices[10] = y; vertices[11] = 0.0; // Bottom right corner
//	vertices[12] = x + width; vertices[13] = y + height; vertices[14] = 0.0; // Bottom left corner
//	vertices[15] = x + width; vertices[16] = y; vertices[17] = 0.0; // Top Right corner
//
//	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
//	glDrawArrays(GL_TRIANGLES, 0, 6);
//
//	delete[] vertices;
//}

void drawTriangle(float* vertArray)
{
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), vertArray, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vboID[ArrayBuffer]);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void initilizePlayer(float x, float y, float height, float width)
{
	vertices[0] = x; vertices[1] = y; vertices[2] = 0.0;
	vertices[3] = x + width; vertices[4] = y; vertices[5] = 0.0;
	vertices[6] = x + (width / 2.0f); vertices[7] = y + height; vertices[8] = 0.0;
}

void handleKeyboardInput(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	switch (key) 
	{
	case GLFW_KEY_D:
		player->moveRight();
		break;
	case GLFW_KEY_A:
		player->moveLeft();
		break;
	case GLFW_KEY_SPACE:
		if (action == GLFW_PRESS)
			player->jump();
		break;
	}
}

void windowResizeCallback(GLFWwindow* window, int width, int height)
{
	glfwSetWindowSize(window, width, height);
}
