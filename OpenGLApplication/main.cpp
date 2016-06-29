#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <stdio.h>
#include "LoadShaders.h"

using namespace std;
using namespace glm;

void initializeDependencies(void);
void createSquare(float x, float y, float width, float height);
void createTriangle(float x, float y, float width, float height);
void redrawScreen(GLFWwindow* window);
void updateGameState();
int getCurrentTime();

enum VAO_IDs { Triangles, VAOSize };
enum Buffer_IDs { ArrayBuffer, BufferSize };
enum Attrib_IDs { vPosition = 0, vOffset = 6 };
enum Game_Loop { MS_PER_UPDATE = 16 };

unsigned int vaoID[1]; // Our Vertex Array Object
unsigned int vboID[1]; // Our Vertex Buffer Object

float xOrigin = 0;
float yOrigin = 0;

GLuint vertexArrays[VAOSize];
GLuint buffers[BufferSize];
const GLuint VerticesCount = 6;
bool isFullscreen = false;

float aspectRatio = 16.0 / 9.0;
int windowHeight = 400;
int windowWidth = windowHeight * aspectRatio;

int main() {

	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Hello Triangle", NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
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

		glfwPollEvents();

		while (lag >= MS_PER_UPDATE)
		{
			updateGameState();
			lag -= MS_PER_UPDATE;
		}

		redrawScreen(window);
	}

	return 1;


	glfwTerminate();
	return 0;
}

void initializeDependencies(void) {

	glClearColor(1, 1, 0, 1);
	glGenVertexArrays(VAOSize, vertexArrays);
	glBindVertexArray(vertexArrays[Triangles]);

	GLfloat vertices[VerticesCount][2] = {
		{ -0.90, -0.90 },
		{ 0.85, -0.90 },
		{ -0.90,  0.85 },
		{ 0.90, -0.85 },
		{ 0.90,  0.90 },
		{ -0.85,  0.90 }
	};

	glGenBuffers(BufferSize, buffers);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[ArrayBuffer]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "C:\\Users\\Christopher\\Documents\\visual studio 2015\\Projects\\OpenGLApplication\\OpenGLApplication\\triangles.vert" },
		{ GL_FRAGMENT_SHADER, "C:\\Users\\Christopher\\Documents\\visual studio 2015\\Projects\\OpenGLApplication\\OpenGLApplication\\triangles.frag" },
		{ GL_NONE, NULL }
	};

	GLuint program = LoadShaders(shaders);
	glUseProgram(program);

	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);
}

void redrawScreen(GLFWwindow* window)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//glBindVertexArray(vertexArrays[Triangles]);
	//glDrawArrays(GL_TRIANGLES, 0, VerticesCount);

	createSquare(0, 0, 1, 1);
	glBindVertexArray(vaoID[0]);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

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

void createSquare(float x, float y, float width, float height)
{
	float* vertices = new float[18];  // Vertices for our square

	vertices[0] = x; vertices[1] = y; vertices[2] = 0.0; // Bottom left corner
	vertices[3] = x; vertices[4] = y + height; vertices[5] = 0.0; // Top left corner
	vertices[6] = x + width; vertices[7] = y + height; vertices[8] = 0.0; // Top Right corner

	vertices[9] = x + width; vertices[10] = y; vertices[11] = 0.0; // Bottom right corner
	vertices[12] = x; vertices[13] = y; vertices[14] = 0.0; // Bottom left corner
	vertices[15] = x + width; vertices[16] = y + height; vertices[17] = 0.0; // Top Right corner

	glGenVertexArrays(1, &vaoID[0]); // Create our Vertex Array Object
	glBindVertexArray(vaoID[0]); // Bind our Vertex Array Object so we can use it

	glGenBuffers(1, vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID[0]);
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	delete[] vertices;
}

void createTriangle(float x, float y, float width, float height)
{

}